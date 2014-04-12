#include "editor/tools/pen_penctil_tool_base.h"

#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"
#include "gui/canvas_widget_t.h"
#include "gui/shortcuts_config.h"

#include "editor/operations/add_item_operation.h"

#include "path/svg_path_geom.h"

#include "svg/items/svg_item_path.h"
#include "svg/svg_document.h"

#include "renderer/path_preview_renderer.h"
#include "renderer/overlay_renderer.h"
#include "renderer/svg_painter.h"
#include "editor/pen_handles.h"
#include "editor/operations/merge_path_operation.h"
#include "editor/items_selection.h"
#include "editor/operations/path_edit_operation.h"

pen_penctil_tool_base::pen_penctil_tool_base (svg_painter *painter)
  : abstract_tool (painter)
{
  m_pen_handles.reset (new pen_handles (m_overlay, m_painter, m_actions_applier));
  m_actions_applier->register_action (gui_action_id::CANCEL_EDITING, this, &pen_penctil_tool_base::cancel_editing);
}

pen_penctil_tool_base::~pen_penctil_tool_base ()
{

}

bool pen_penctil_tool_base::finish_path_add ()
{
  if (!edit_started ())
    return false;

  svg_item_path *cur_path = add_new_path ();
  if (cur_path == nullptr)
    return false;

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

  finish_editing ();
  m_painter->document ()->apply_changes (undo_description ());
  return true;
}

void pen_penctil_tool_base::deactivate ()
{
  finish_path_add ();
}

void pen_penctil_tool_base::configure ()
{
  if (m_painter->get_configure_needed (configure_type::SELECTION_CHANGED))
    m_pen_handles->update_handles ();
}

void pen_penctil_tool_base::activate ()
{
  m_pen_handles->update_handles ();
}

QPointF pen_penctil_tool_base::snap_point (QPointF point)
{
  svg_path_geom_iterator it;
  svg_item_path *path = 0;
  if (!m_pen_handles->get_path_by_pos (point, it, path))
    return m_painter->get_local_pos (point);

  if (edit_started ())
    m_path_snap_end.reset (new snap_point_t (path, it));
  else
    m_path_snap_start.reset (new snap_point_t (path, it));


  return path ? path->full_transform ().map (it.anchor_point ()) : it.anchor_point ();
}

svg_item_path *pen_penctil_tool_base::add_new_path ()
{
  auto path_item = m_painter->document ()->create_new_svg_item<svg_item_path> ();
  if (path_item == nullptr)
    return nullptr;

  add_item_operation (m_painter).apply (path_item);
  set_new_path (path_edit_operation (path_item).get_svg_path ());

  return path_item;
}

svg_item_path *pen_penctil_tool_base::merge_with_path (svg_item_path *path_dst, svg_path_geom_iterator dst_it, svg_item_path *path_src, svg_path_geom_iterator src_it)
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

bool pen_penctil_tool_base::cancel_editing ()
{
  if (!edit_started ())
    return false;

  finish_editing ();
  return true;
}

void pen_penctil_tool_base::finish_editing ()
{
  m_path_snap_end.reset ();
  m_path_snap_start.reset ();
  m_pen_handles->set_new_path (nullptr);
  finish_editing_impl ();
  update ();
}




