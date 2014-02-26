#include "renderer/renderer_items_container.h"

#include "common/memory_deallocation.h"
#include "common/debug_utils.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/render_cache_id.h"
#include "renderer/rendered_items_cache.h"



renderer_items_container::renderer_items_container ()
{
  m_last_id = (int)render_cache_type::OBJECTS_ID_COUNT;
  m_cache = nullptr;
}

renderer_items_container::~renderer_items_container ()
{
  for (const auto &pair : m_items)
    {
      abstract_renderer_item *item = pair.second;
      FREE (item);
    }
}

abstract_renderer_item *renderer_items_container::item (const string &name)
{
  auto it = m_items.find (name);
  if (it == m_items.end ())
    return nullptr;

  return it->second;
}

const abstract_renderer_item * renderer_items_container::item (const string &name) const
{
  auto it = m_items.find (name);
  if (it == m_items.end ())
    return nullptr;

  return it->second;
}

void renderer_items_container::set_root (const string &root)
{
  m_root = root;
}

const abstract_renderer_item *renderer_items_container::root () const
{
  return item (m_root);
}

abstract_renderer_item *renderer_items_container::root ()
{
  return item (m_root);
}

void renderer_items_container::add_item (abstract_renderer_item *item)
{
  auto it = m_items.find (item->name ());
  DEBUG_ASSERT (it == m_items.end ());

  item->set_container (this);
  item->set_unique_id (m_last_id++);
  if (m_cache)
    m_cache->add_selection_mapping (item->unique_id (), item->name ());
  m_items.insert (std::make_pair (item->name (), item));
}

void renderer_items_container::remove_item (const string &name)
{
  auto it = m_items.find (name);
  if (it == m_items.end ())
    return;

  if (m_cache)
    m_cache->remove_selection_mapping (it->second->unique_id ());
  abstract_renderer_item *item = it->second;
  FREE (item);
  m_items.erase (it);
}

void renderer_items_container::add_child (const string &parent, const string &child)
{
  auto parent_it = m_items.find (parent);
  auto child_it = m_items.find (child);
  DEBUG_ASSERT (parent_it != m_items.end () && child_it != m_items.end ());
  parent_it->second->push_back_child (child);
  child_it->second->set_parent (parent);
}

void renderer_items_container::change_item (abstract_renderer_item *item)
{
  abstract_renderer_item *prev_item = m_items[item->name ()];
  int unique_id;
  if (prev_item)
    {
      unique_id = prev_item->unique_id ();
      FREE (prev_item);
    }
  else
    {
      unique_id = m_last_id++;
      if (m_cache)
        m_cache->add_selection_mapping (unique_id, item->name ());
    }

  m_items[item->name ()] = item;

  item->set_container (this);
  item->set_unique_id (unique_id);
}
