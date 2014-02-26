#include "renderer/event_items_changed.h"

#include "renderer/renderer_thread.h"
#include "renderer/renderer_items_container.h"
#include "renderer/abstract_renderer_item.h"

event_items_changed::event_items_changed ()
{

}

event_items_changed::~event_items_changed ()
{

}

void event_items_changed::add_renderer_item (abstract_renderer_item *item)
{
  m_changed_items.push_back (item);
}

void event_items_changed::add_removed_item (const string &item)
{
  m_removed_items.push_back (item);
}

void event_items_changed::add_layout_changed_item (abstract_renderer_item *item)
{
  m_layout_changed_items.push_back (item);
}


void event_items_changed::process (renderer_thread *renderer)
{
  renderer_items_container *container = renderer->container ();
  for (abstract_renderer_item *item : m_changed_items)
    {
      change_item (renderer, item, true);
      item->update_bbox ();
      renderer->invalidate_rect (item->bounding_box ());
    }

  for (abstract_renderer_item *item : m_layout_changed_items)
    {
      change_item (renderer, item, false);
      item->update_bbox ();
      renderer->invalidate_rect (item->bounding_box ());
    }

  for (const string &name : m_removed_items)
    {
      abstract_renderer_item *item_removed = container->item (name);
      if (!item_removed)
        continue;

      abstract_renderer_item *parent = item_removed->parent ();
      renderer->invalidate_rect (item_removed->bounding_box ());
      container->remove_item (name);
      if (parent)
        parent->invalidate_bbox ();
    }

  container->root ()->update_bbox ();
}

void event_items_changed::change_item (renderer_thread *renderer, abstract_renderer_item *item, bool invalidate_prev_bbox)
{
  renderer_items_container *container = renderer->container ();
  if (invalidate_prev_bbox)
    {
      abstract_renderer_item *prev_item = container->item (item->name ());
      if (prev_item)
        renderer->invalidate_rect (prev_item->bounding_box ());
    }

  container->change_item (item);
  item->invalidate_bbox ();
}
