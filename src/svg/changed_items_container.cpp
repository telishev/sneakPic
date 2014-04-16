#include "svg/changed_items_container.h"

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"

#include "renderer/event_items_changed.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/svg_graphics_item.h"

#include "svg/undo/undo_handler.h"


items_edit_handler_t::items_edit_handler_t (svg_items_container *container)
{
  m_container = container;
  m_undo_handler = new undo_handler;
}

items_edit_handler_t::~items_edit_handler_t ()
{
  FREE (m_undo_handler);
}

void items_edit_handler_t::child_added (const string &parent, const string &child_name, int insert_pos)
{
  m_changed_items.insert (child_name);
  m_layout_changed_items.insert (parent);
  emit child_added_signal (parent, child_name, insert_pos);
}

void items_edit_handler_t::child_removed (const string &parent, const string &child_name, int pos)
{
  m_removed_items.insert (child_name);
  m_layout_changed_items.insert (parent);
  emit child_removed_signal (parent, child_name, pos);
}

void items_edit_handler_t::child_moved (const string &parent, const string &child_name, int old_pos, int new_pos)
{
  m_layout_changed_items.insert (parent);
  emit child_moved_signal (parent, child_name, old_pos, new_pos);
}

void items_edit_handler_t::attribute_change_start (const string &parent, const abstract_attribute *computed_attribute)
{
  set_children_changed (parent);
  emit attribute_change_start_signal (parent, computed_attribute);
}

void items_edit_handler_t::attribute_change_end (const string &parent, const abstract_attribute *computed_attribute)
{
  emit attribute_change_end_signal (parent, computed_attribute);
}

void items_edit_handler_t::item_removed (const string &parent)
{
  emit item_removed_signal (parent);
  /// handled in child_removed
}

void items_edit_handler_t::layout_changed (const string &sender)
{
  set_children_changed (sender);
  m_layout_changed_items.insert (sender);
}

abstract_state_t *items_edit_handler_t::create_state ()
{
  DEBUG_PAUSE ("Not really undoable");
  return nullptr;
}

void items_edit_handler_t::load_from_state (const abstract_state_t * /*state*/)
{
  DEBUG_PAUSE ("Not really undoable");
}

event_items_changed *items_edit_handler_t::create_changed_items_event (bool need_to_clear)
{
  event_items_changed *ev = new event_items_changed;
  for (const string &item_name : m_changed_items)
    invalidate_bbox (item_name);

  for (const string &item_name : m_layout_changed_items)
    invalidate_bbox (item_name);

  m_container->get_root ()->to_graphics_item ()->update_bbox ();

  for (const string &item_name : m_changed_items)
    {
      svg_graphics_item *graphics_item = get_graphics_item (item_name);
      if (!graphics_item)
        continue;

      ev->add_renderer_item (graphics_item->create_renderer_item ());
    }

  for (const string &item_name : m_layout_changed_items)
    {
      if (m_changed_items.find (item_name) != m_changed_items.end ())
        continue;

      svg_graphics_item *graphics_item = get_graphics_item (item_name);
      if (!graphics_item)
        continue;

      ev->add_layout_changed_item (graphics_item->create_renderer_item ());
    }

  for (const string &item_name : m_removed_items)
    {
      ev->add_removed_item (item_name);
    }

  if (need_to_clear)
    clear ();
  return ev;
}

void items_edit_handler_t::clear ()
{
  m_changed_items.clear ();
  m_layout_changed_items.clear ();
  m_removed_items.clear ();
}

void items_edit_handler_t::invalidate_bbox (const string &item_name)
{
  svg_graphics_item *graphics_item = get_graphics_item (item_name);
  if (!graphics_item)
    return;

  graphics_item->invalidate_bbox ();
}

svg_graphics_item *items_edit_handler_t::get_graphics_item (const string &item_name) const
{
  abstract_svg_item *item = m_container->get_item (item_name);
  return item ? item->to_graphics_item () : nullptr;
}

void items_edit_handler_t::set_item_changed (const string &item)
{
  set_children_changed (item);
  m_removed_items.erase (item);
}

void items_edit_handler_t::set_item_layout_changed (const string &item)
{
  m_layout_changed_items.insert (item);
  m_removed_items.erase (item);
}

void items_edit_handler_t::set_item_removed (const string &item)
{
  m_removed_items.insert (item);
}

void items_edit_handler_t::set_children_changed (const string &parent_name)
{
  m_changed_items.insert (parent_name);
  abstract_svg_item *item = m_container->get_item (parent_name);
  if (!item)
    return;

  for (int i = 0; i < item->children_count (); i++)
    if (item->child (i))
      set_children_changed (item->child (i)->name ());
}
