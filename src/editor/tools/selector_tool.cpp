#include "editor/tools/selector_tool.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"
#include "editor/items_move_handler.h"
#include "editor/transform_handles_editor.h"

#include "gui/actions_applier.h"
#include "gui/settings.h"

#include "renderer/svg_painter.h"
#include "renderer/overlay_renderer.h"
#include "renderer/items_rubberband_selector.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"
#include "gui/connection.h"
#include "editor/selection_helpers.h"
#include "editor/item_helpers.h"




selector_tool::selector_tool (svg_painter *painter)
  : abstract_tool (painter)
{
  m_rubberband = new items_rubberband_selector (m_overlay, m_painter, m_actions_applier);
  m_move_handler = new items_move_handler (m_painter->item_container (), m_overlay, m_painter->selection (), m_painter->document ());
  put_in (m_transform_handles_editor, m_overlay, m_painter, m_actions_applier);

  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_t::DRAG_OBJECTS, this,
    &selector_tool::start_moving_object, &selector_tool::move_object, &selector_tool::end_moving_object);

  m_actions_applier->add_shortcut (mouse_shortcut_t::SELECT_ITEM_OR_GROUP, this, &selector_tool::select_object);
}

void  selector_tool::change_handles_type ()
{
  m_transform_handles_editor->switch_handles_type ();
}

selector_tool::~selector_tool ()
{
  FREE (m_rubberband);
  FREE (m_move_handler);
}

bool selector_tool::start_moving_object (const QPoint &pos)
{
  abstract_svg_item *current_item = get_item_or_group_for_pos (pos);
  if (!current_item)
    return false;

  items_selection *selection = m_painter->selection ();
  if (!selection->contains (current_item->name ()))
    {
      selection->clear ();
      selection->add_item (current_item);
    }

  QPointF cur_point = m_painter->get_local_pos (QPointF (pos));
  m_move_handler->start_move (cur_point);
  m_painter->redraw ();
  return true;
}

bool selector_tool::move_object (const QPoint &pos)
{
  QPointF cur_point = m_painter->get_local_pos (QPointF (pos));
  m_move_handler->move (cur_point);
  m_painter->redraw ();
  return true;
}

bool selector_tool::end_moving_object (const QPoint &)
{
  m_move_handler->end_move ();
  update_handles ();
  m_painter->redraw ();
  return true;
}

void selector_tool::activate ()
{
  update_handles ();
}

void selector_tool::update_handles ()
{
  m_transform_handles_editor->update_handles ();
}

void selector_tool::deactivate ()
{
}

void selector_tool::configure ()
{
  if (m_painter->get_configure_needed (configure_type::SELECTION_CHANGED))
    {
      if (m_painter->selection ()->empty ())
        m_transform_handles_editor->reset_handles_type ();
      update_handles ();
    }
}

bool selector_tool::select_object (const mouse_event_t &event)
{
  abstract_svg_item *item = get_item_or_group_for_pos (event.pos ());
  bool add_to_selection = contains_modifier (event.modifier (), keyboard_modifier::SHIFT);
  items_selection *selection = m_painter->selection ();

  if (item && selection->contains (item->name ()) && !add_to_selection)
    {
      m_transform_handles_editor->switch_handles_type ();
      return true;
    }

  selection_helpers (selection).select (item, add_to_selection);

  m_painter->redraw ();
  return true;
}

abstract_svg_item * selector_tool::get_item_or_group_for_pos (const QPoint &pos) const
{
  return item_helpers (m_painter->document ()).get_selectable_item_or_group (m_painter->get_current_item (pos));
}
