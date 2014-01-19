#include "editor/tools/pen_tool.h"

#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"
#include "gui/shortcuts_config.h"

#include "editor/operations/add_item_operation.h"

#include "path/svg_path.h"
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




pen_tool::pen_tool (svg_painter *painter)
  : abstract_tool (painter)
{
  m_preview_renderer.reset (new path_preview_renderer (0, QColor ("slateblue")));
  m_anchor_renderer.reset (new anchor_handle_renderer);
  m_left_cp_renderer.reset (new path_control_point_renderer);
  m_right_cp_renderer.reset (new path_control_point_renderer);
  
  m_overlay->add_item (m_preview_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_anchor_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_left_cp_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_right_cp_renderer.get (), overlay_layer_type::TEMP);

  m_actions_applier->add_shortcut (mouse_shortcut_enum::PEN_ADD_SEGMENT_SIMPLE, this, &pen_tool::add_segment_simple);

  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_enum::PEN_ADD_SEGMENT_DRAG, this,
    &pen_tool::add_segment_start, &pen_tool::add_segment_move, &pen_tool::add_segment_end);

  m_actions_applier->register_action (gui_action_id::FINISH_PATH, this, &pen_tool::finish_path_add);
}

pen_tool::~pen_tool ()
{

}

bool pen_tool::add_segment_simple (const QPoint &pos)
{
  QPointF local_pos = m_painter->get_local_pos (pos);
  if (!m_current_path)
    {
      m_current_path.reset (new svg_path);
      m_preview_renderer->set_path (m_current_path.get ());
      m_path_builder.reset (new path_builder (*m_current_path));
      m_path_builder->move_to (local_pos, false);
      m_path_builder->check_new_subpath ();

      m_anchor_renderer->set_visible (true);
      m_anchor_renderer->set_pos (local_pos);
    }
  else
    m_path_builder->curve_to_short (local_pos, local_pos, false);

  m_left_cp_renderer->set_visible (true);
  m_left_cp_renderer->set_anchor (local_pos);
  m_left_cp_renderer->set_control_point (local_pos);
  m_right_cp_renderer->set_visible (true);
  m_right_cp_renderer->set_anchor (local_pos);
  m_right_cp_renderer->set_control_point (local_pos);

  update ();
  return true;
}

bool pen_tool::add_segment_start (const QPoint &pos)
{
  return add_segment_simple (pos);
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
  return add_segment_move (pos);
}

bool pen_tool::finish_path_add ()
{
  if (!m_current_path)
    return false;

  auto *svg_item = m_painter->document ()->create_new_svg_item<svg_item_path> ();

  *svg_item->get_attribute_for_change<svg_attribute_path_data> ()->path () = *m_current_path;
  svg_item->process_item_after_read ();

  add_item_operation (m_painter).apply (svg_item);

  m_current_path.reset ();
  m_preview_renderer->set_path (nullptr);
  m_anchor_renderer->set_visible (false);
  m_left_cp_renderer->set_visible (false);
  m_right_cp_renderer->set_visible (false);
  update ();
  m_painter->document ()->apply_changes ();
  return true;
}

void pen_tool::deactivate ()
{
  finish_path_add ();
}
