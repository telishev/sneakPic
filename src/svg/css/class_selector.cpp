#include "svg/css/class_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"

class_selector::class_selector (const std::string &class_name)
{
  m_class_name = class_name;
}

bool class_selector::is_matched (const abstract_svg_item *item) const 
{
  return item->is_xml_class (m_class_name);
}
