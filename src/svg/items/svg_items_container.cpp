#include "svg/items/svg_items_container.h"

#include "common/debug_utils.h"
#include "common/string_utils.h"

#include "svg/items/abstract_svg_item.h"


svg_items_container::svg_items_container ()
{
  max_id = 0;
}

svg_items_container::~svg_items_container ()
{

}

void svg_items_container::add_item (abstract_svg_item *item)
{
  std::string id = item->name ();
  extract_number (id.c_str ());
  DEBUG_ASSERT (!id.empty ());
  DEBUG_ASSERT (!contains (id));

  m_map.insert (std::make_pair (id, item));
}

void svg_items_container::remove_item (abstract_svg_item *item)
{
  std::string id = item->name ();
  DEBUG_ASSERT (!id.empty ());
  DEBUG_ASSERT (contains (id));
  m_map.erase (id);
}

abstract_svg_item *svg_items_container::get_item (const std::string &id) const
{
  auto it = m_map.find (id);
  if (it == m_map.end ())
    return nullptr;

  return it->second;
}

abstract_svg_item *svg_items_container::get_editable_item (const std::string &id) const
{
  abstract_svg_item *item = get_item (id);
  if (!item)
    return nullptr;

  while (item && item->is_cloned ())
    item = item->parent ();

  return item;
}

bool svg_items_container::contains (const std::string &id) const
{
  return m_map.find (id) != m_map.end ();
}

std::string svg_items_container::create_unique_name (const char *item_id)
{
  max_id++;
  return std::string (item_id) + std::to_string (max_id);
}

void svg_items_container::extract_number (const char *data)
{
  while (!is_num (*data) && *data)
    data++;

  if (!*data)
    return;

  int number = atoi (data);
  max_id = std::max (max_id, number);
}

void svg_items_container::set_root (const std::string &root_id)
{
  m_root = root_id;
}

abstract_svg_item *svg_items_container::get_root () const
{
  return get_item (m_root);
}
