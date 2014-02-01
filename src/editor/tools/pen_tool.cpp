#include "editor/tools/pen_tool.h"

#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"
#include "gui/shortcuts_config.h"

#include "editor/operations/add_item_operation.h"

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
#include "editor/pen_handles.h"
#include "editor/operations/merge_path_operation.h"
#include "editor/items_selection.h"
#include "editor/operations/path_edit_operation.h"




pen_tool::pen_tool (svg_painter *painter)
  : abstract_tool (painter)
{
  m_pen_handles.reset (new pen_handles (m_overlay, m_painter, m_actions_applier));
  m_preview_renderer.reset (new path_preview_renderer (0, QColor ("slateblue")));
  m_left_cp_renderer.reset (new path_control_point_renderer);
  m_right_cp_renderer.reset (new path_control_point_renderer);
  
  m_overlay->add_item (m_preview_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_left_cp_renderer.get (), overlay_layer_type::TEMP);
  m_overlay->add_item (m_right_cp_renderer.get (), overlay_layer_type::TEMP);

  m_actions_applier->add_shortcut (mouse_shortcut_enum::PEN_ADD_SEGMENT_SIMPLE, this, &pen_tool::add_segment_simple);

  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_enum::PEN_ADD_SEGMENT_DRAG, this,
    &pen_tool::add_segment_start, &pen_tool::add_segment_move, &pen_tool::add_segment_end);

  m_actions_applier->register_action (gui_action_id::FINISH_PATH, this, &pen_tool::finish_path_add);
  m_prev_point_was_line = false;
}

pen_tool::~pen_tool ()
{

}

bool pen_tool::add_segment_simple (const QPoint &pos)
{
  add_new_point (pos, true);
  if (m_path_snap_end)
    finish_path_add ();

  update ();
  return true;
}

bool pen_tool::add_segment_start (const QPoint &pos)
{
  add_new_point (pos, false);
  update ();
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

bool pen_tool::finish_path_add ()
{
  if (!m_current_path)
    return false;

  svg_item_path *cur_path = add_new_path ();

  if (m_path_snap_start)
    cur_path = merge_with_path (m_path_snap_start->first, m_path_snap_start->second, cur_path, cur_path->get_svg_path ()->begin ());

  if (m_path_snap_end)
    {
      if (m_path_snap_start)
        cur_path = merge_with_path (cur_path, cur_path->get_svg_path ()->last_point (), m_path_snap_end->first, m_path_snap_end->second);
      else
        {
          /// if path item is nullptr it means it was part of new path
          if (m_path_snap_end->first)
            cur_path = merge_with_path (m_path_snap_end->first, m_path_snap_end->second, cur_path, cur_path->get_svg_path ()->last_point ());
          else
            cur_path = merge_with_path (cur_path, cur_path->get_svg_path ()->last_point (), cur_path, cur_path->get_svg_path ()->last_point ());
        }
    }

  if (cur_path)
    {
      m_painter->selection ()->clear ();
      m_painter->selection ()->add_item (cur_path);
    }

  m_current_path.reset ();
  m_path_snap_end.reset ();
  m_path_snap_start.reset ();
  m_preview_renderer->set_path (nullptr);
  m_left_cp_renderer->set_visible (false);
  m_right_cp_renderer->set_visible (false);
  m_pen_handles->set_new_path (nullptr);
  update ();
  m_painter->document ()->apply_changes ();
  return true;
}

void pen_tool::deactivate ()
{
  finish_path_add ();
}

void pen_tool::configure ()
{
  if (m_painter->get_configure_needed (configure_type::SELECTION_CHANGED))
    m_pen_handles->update_handles ();
}

void pen_tool::activate ()
{
  m_pen_handles->update_handles ();
}

QPointF pen_tool::snap_point (QPointF point)
{
  svg_path_geom_iterator it;
  svg_item_path *path = 0;
  if (!m_pen_handles->get_path_by_pos (point, it, path))
    return m_painter->get_local_pos (point);
 
  if (m_current_path)
    m_path_snap_end.reset (new snap_point_t (path, it));
  else
    m_path_snap_start.reset (new snap_point_t (path, it));


  return path ? path->full_transform ().map (it.anchor_point ()) : it.anchor_point ();
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

svg_item_path *pen_tool::add_new_path ()
{
  auto path_item = m_painter->document ()->create_new_svg_item<svg_item_path> ();
  add_item_operation (m_painter).apply (path_item);
  path_edit_operation (path_item).get_svg_path ()->copy_from (*m_current_path->path ());
  
  return path_item;
}

svg_item_path *pen_tool::merge_with_path (svg_item_path *path_dst, svg_path_geom_iterator dst_it, svg_item_path *path_src, svg_path_geom_iterator src_it)
{
  {
    path_edit_operation dst (path_dst);
    path_edit_operation src (path_src);
    merge_path_operation ().apply (dst.get_svg_path (), dst_it, src.get_svg_path (), src_it);
  }

  if (path_src != path_dst && path_src->parent ())
    path_src->parent ()->remove_child (path_src);

  return path_dst;
}


