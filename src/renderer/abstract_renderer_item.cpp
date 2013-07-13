#include "renderer/abstract_renderer_item.h"

#include "renderer/renderer_items_container.h"




abstract_renderer_item::abstract_renderer_item (const std::string &name)
{
  m_name = name;
  m_container = nullptr;
  m_unique_id = -1;
}

abstract_renderer_item::~abstract_renderer_item ()
{
}

void abstract_renderer_item::push_back_child (const std::string &child)
{
  m_children.push_back (child);
}

void abstract_renderer_item::erase_child (const std::string &child)
{
  m_children.erase (std::remove (m_children.begin (), m_children.end (), child), m_children.end ());
}
