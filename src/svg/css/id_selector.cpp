#include "svg/css/id_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"

id_selector::id_selector (const std::string &id_name)
{
  m_id_name = id_name;
}

bool id_selector::is_matched (const abstract_svg_item *item) const 
{
  return item->name () == m_id_name;
}

