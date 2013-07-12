#include "renderer/renderer_items_container.h"

#include "common/memory_deallocation.h"
#include "common/debug_utils.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/render_cache_id.h"
#include "renderer/rendered_items_cache.h"



renderer_items_container::renderer_items_container ()
{
  m_last_id = render_cache_id::OBJECTS_ID_COUNT;
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

abstract_renderer_item *renderer_items_container::item (const std::string &name)
{
  auto it = m_items.find (name);
  if (it == m_items.end ())
    return nullptr;

  return it->second;
}

const abstract_renderer_item * renderer_items_container::item (const std::string &name) const
{
  auto it = m_items.find (name);
  if (it == m_items.end ())
    return nullptr;

  return it->second;
}

void renderer_items_container::set_root (const std::string &root)
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

void renderer_items_container::remove_item (const std::string &name)
{
  auto it = m_items.find (name);
  DEBUG_ASSERT (it != m_items.end ());

  if (m_cache)
    m_cache->remove_selection_mapping (it->second->unique_id ());
  abstract_renderer_item *item = it->second;
  FREE (item);
  m_items.erase (it);
}

void renderer_items_container::add_child (const std::string &parent, const std::string &child)
{
  auto parent_it = m_items.find (parent);
  DEBUG_ASSERT (parent_it != m_items.end ());
  parent_it->second->push_back_child (child);
}
