#include "editor/tools/path_editor_tool.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/settings.h"
#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"

#include "renderer/svg_painter.h"
#include "renderer/rubberband_selection.h"
#include "renderer/overlay_renderer.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"

path_editor_tool::path_editor_tool (svg_painter *painter)
  : abstract_tool (painter)
{
  m_rubberband = new rubberband_selection (m_overlay);

  ADD_SHORTCUT_DRAG (m_mouse_handler, RUBBERBAND_SELECTION,
    return start_rubberband_selection (m_event.pos ()),
    return move_rubberband_selection (m_event.pos ()),
    return end_rubberband_selection (m_event));

  m_actions_applier->register_action (gui_action_id::DELETE_HANDLES, this, &path_editor_tool::delete_handles);
}

path_editor_tool::~path_editor_tool ()
{
  FREE (m_rubberband);
}

bool path_editor_tool::start_rubberband_selection (const QPoint &pos)
{
  QPointF start_point = m_painter->get_local_pos (QPointF (pos));
  m_rubberband->start_selection (start_point.x (), start_point.y ());
  m_painter->redraw ();
  return true;
}

bool path_editor_tool::move_rubberband_selection (const QPoint &pos)
{
  QPointF cur_point = m_painter->get_local_pos (QPointF (pos));
  m_rubberband->move_selection (cur_point.x (), cur_point.y ());
  m_painter->redraw ();
  return true;
}

bool path_editor_tool::end_rubberband_selection (const mouse_event_t &event)
{
  items_selection *selection = m_painter->selection ();
  if (event.modifier () != keyboard_modifier::SHIFT)
    selection->clear ();

  selection->add_items_for_rect (m_rubberband->selection_rect (), m_painter->item_container ()->get_root ());
  m_rubberband->end_selection ();
  m_painter->redraw ();
  return true;
}

bool path_editor_tool::delete_handles ()
{
  return true;
}
