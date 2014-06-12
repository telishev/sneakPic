#include "editor/tools/pen_tool.h"

#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"
#include "gui/connection.h"
#include "gui/canvas_widget_t.h"
#include "gui/shortcuts_config.h"
#include "gui/settings.h"

#include "path/svg_path.h"
#include "path/svg_path_geom.h"
#include "path/path_builder.h"

#include "svg/items/svg_item_path.h"
#include "svg/svg_document.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/svg_attribute_nodetypes.h"

#include "renderer/renderable_item.h"
#include "renderer/path_preview_renderer.h"
#include "renderer/overlay_renderer.h"
#include "renderer/svg_painter.h"
#include "renderer/anchor_handle_renderer.h"
#include "renderer/path_control_point_renderer.h"
#include "editor/items_selection.h"
#include "editor/operations/path_edit_operation.h"
#include "editor/pen_handles.h"

pen_tool::pen_tool (svg_painter *painter)
  : pen_penctil_tool_base (painter)
{
  m_preview_renderer.reset (new path_preview_renderer (0, QColor ("slateblue")));
  m_auxiliary_preview_renderer.reset (new path_preview_renderer (0, QColor ("red")));
  m_left_cp_renderer.reset (new path_control_point_renderer);
  m_right_cp_renderer.reset (new path_control_point_renderer);

  m_overlay->add_item (m_preview_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_auxiliary_preview_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_left_cp_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_right_cp_renderer.get (), overlay_layer_type::TEMP);

  m_actions_applier->add_shortcut (mouse_shortcut_t::PEN_ADD_SEGMENT_SIMPLE, this, &pen_tool::add_segment_simple);
  m_actions_applier->add_shortcut (mouse_shortcut_t::UPDATE_AUXILIARY_PEN_PREVIEW, this, &pen_tool::update_auxiliary_pen_preview);

  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_t::PEN_ADD_SEGMENT_DRAG, this,
    &pen_tool::add_segment_start, &pen_tool::add_segment_move, &pen_tool::add_segment_end);

  m_actions_applier->register_action (gui_action_id::FINISH_PATH, std::bind (&pen_tool::finish_path_add, this));
  m_actions_applier->register_action (gui_action_id::CANCEL_CURVE, this, &pen_tool::cancel_curve);
  m_prev_point_was_line = false;

  CONNECT (m_painter->settings (), &settings_t::settings_changed, this, &pen_tool::update_on_settings_changed);
}

pen_tool::~pen_tool ()
{

}

void pen_tool::update_on_settings_changed ()
{
  update_auxiliary_pen_preview ();
}

bool pen_tool::update_auxiliary_pen_preview (const QPoint &pos)
{
  QPoint dest_pos;
  if (!m_painter->settings ()->value (option_id::NEXT_CURVE_PREVIEW).toBool ())
    {
      m_auxiliary_preview_renderer->set_path (nullptr);
      return true;
    }

  if (pos.isNull ())
    dest_pos = m_painter->canvas_widget ()->cursor_pos ();
  else
    dest_pos = pos;

  if (m_current_path)
    {
      QPointF local_pos = m_painter->get_local_pos (dest_pos);
      svg_path_geom *cur_path = m_current_path->path ()->get_geom ();
      m_auxiliary_path.reset (new unique_svg_path);
      m_auxiliary_preview_renderer->set_path (m_auxiliary_path->path ()->get_geom ());
      path_builder builder (*m_auxiliary_path->path ());
      builder.move_to (cur_path->last_point ().anchor_point (), false);
      if (cur_path->total_points () > 0)
        builder.set_prev_curve_c (cur_path->last_point ().control_point (cp_type::LEFT));
      builder.curve_to_short (local_pos, local_pos, false);
       m_painter->update ();
      return true;
    }

  return false;
}

bool pen_tool::add_segment_simple (const QPoint &pos)
{
  add_new_point (pos, true);
  if (m_path_snap_end)
    finish_path_add ();

  update_auxiliary_pen_preview ();
  update ();
  return true;
}

bool pen_tool::add_segment_start (const QPoint &pos)
{
  add_new_point (pos, false);
  update ();
  m_auxiliary_preview_renderer->set_path (nullptr);
  return true;
}

bool pen_tool::add_segment_move (const QPoint &pos)
{
  if (!m_current_path || !m_path_builder)
    return false;

  QPointF local_pos = m_painter->get_local_pos (pos);
  m_path_builder->set_curve_c (local_pos);
  m_left_cp_renderer->set_control_point (2 * m_left_cp_renderer->anchor_pos () - local_pos);
  m_right_cp_renderer->set_control_point (local_pos);

  update ();
  return true;
}

bool pen_tool::add_segment_end (const QPoint &pos)
{
  if (!add_segment_move (pos))
    return false;

  if (m_path_snap_end)
    finish_path_add ();

  return true;
}

void pen_tool::add_new_point (QPoint pos, bool is_line)
{
  QPointF local_pos = snap_point (pos);
  if (!m_current_path)
    {
      m_current_path.reset (new unique_svg_path);
      m_preview_renderer->set_path (m_current_path->path ()->get_geom ());

      m_path_builder.reset (new path_builder (*m_current_path->path ()));
      m_path_builder->move_to (local_pos, false);
      m_path_builder->check_new_subpath ();

      m_pen_handles->set_new_path (m_current_path->path ()->get_geom ());
    }
  else
    {
      if (is_line && m_prev_point_was_line)
        m_path_builder->line_to (local_pos, false);
      else
        m_path_builder->curve_to_short (local_pos, local_pos, false);
    }

  m_prev_point_was_line = is_line;
  m_left_cp_renderer->set_visible (true);
  m_left_cp_renderer->set_anchor (local_pos);
  m_left_cp_renderer->set_control_point (local_pos);
  m_right_cp_renderer->set_visible (true);
  m_right_cp_renderer->set_anchor (local_pos);
  m_right_cp_renderer->set_control_point (local_pos);
}

bool pen_tool::cancel_curve ()
{
  if (!m_current_path)
    return false;

  auto geom = m_current_path->path ()->get_geom ();
  geom->erase (geom->last_point ());

  if (m_current_path->path ()->get_geom ()->total_points () == 0)
    {
      finish_editing ();
      return true;
    }
  else if (m_current_path->path ()->get_geom ()->total_points () > 0)
    m_path_builder.reset (new path_builder (*m_current_path->path ()));

  update_cp_renderers ();
  update_auxiliary_pen_preview ();

  update ();
  return true;
}

void pen_tool::update_cp_renderers ()
{

  auto last_point = m_current_path->path ()->get_geom ()->last_point ();
  if (last_point.has_control_point (cp_type::LEFT))
    {
      m_left_cp_renderer->set_visible (true);
      m_left_cp_renderer->set_anchor (last_point.anchor_point ());
      m_left_cp_renderer->set_control_point (last_point.control_point (cp_type::LEFT));
      m_right_cp_renderer->set_visible (true);
      m_right_cp_renderer->set_anchor (last_point.anchor_point ());
      m_right_cp_renderer->set_control_point (last_point.control_point (cp_type::RIGHT));
    }
  else
    {
      m_left_cp_renderer->set_visible (false);
      m_right_cp_renderer->set_visible (false);
    }
}

void pen_tool::set_new_path (svg_path *path)
{
  path->copy_from (*m_current_path->path ());
}

QString pen_tool::undo_description () const
{
  return "Draw Path";
}

bool pen_tool::edit_started () const
{
  return m_current_path != 0;
}

void pen_tool::finish_editing_impl ()
{
  m_current_path.reset ();
  m_auxiliary_preview_renderer->set_path (nullptr);
  m_left_cp_renderer->set_visible (false);
  m_right_cp_renderer->set_visible (false);
  m_preview_renderer->set_path (nullptr);
}


