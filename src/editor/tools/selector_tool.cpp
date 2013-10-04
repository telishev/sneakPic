#include "editor/tools/selector_tool.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/settings.h"

#include "renderer/svg_painter.h"
#include "renderer/rubberband_selection.h"



selector_tool::selector_tool (svg_painter *painter)
{
  m_painter = painter;
  m_mouse_handler = new mouse_shortcuts_handler (m_painter->settings ()->shortcuts_cfg ());
  m_rubberband = new rubberband_selection (m_painter->overlay ());

  ADD_SHORTCUT_DRAG (m_mouse_handler, RUBBERBAND_SELECTION,
    return start_rubberband_selection (m_event.pos ()),
    return move_rubberband_selection (m_event.pos ()),
    return end_rubberband_selection (m_event));
}

selector_tool::~selector_tool ()
{
  FREE (m_rubberband);
  FREE (m_mouse_handler);
}

void selector_tool::activate ()
{

}

void selector_tool::deactivate ()
{

}

bool selector_tool::mouse_event (const mouse_event_t &m_event)
{
  return m_mouse_handler->process_mouse_event (m_event);
}

bool selector_tool::start_rubberband_selection (const QPoint &pos)
{
  QPointF start_point = m_painter->get_local_pos (QPointF (pos));
  m_rubberband->start_selection (start_point.x (), start_point.y ());
  m_painter->redraw ();
  return true;
}

bool selector_tool::move_rubberband_selection (const QPoint &pos)
{
  QPointF cur_point = m_painter->get_local_pos (QPointF (pos));
  m_rubberband->move_selection (cur_point.x (), cur_point.y ());
  m_painter->redraw ();
  return true;
}

bool selector_tool::end_rubberband_selection (const mouse_event_t &event)
{
  items_selection *selection = m_painter->selection ();
  if (event.modifier () != keyboard_modifier::SHIFT)
    selection->clear ();

  selection->add_items_for_rect (m_rubberband->selection_rect ());
  m_rubberband->end_selection ();
  m_painter->redraw ();
  return true;
}

void selector_tool::items_changed ()
{
  m_rubberband->update_items ();
}

