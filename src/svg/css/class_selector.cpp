#include "svg/css/class_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"

#include "svg/attributes/svg_attribute_class.h"

class_selector::class_selector (const string &class_name)
{
  m_class_name = class_name;
}

bool class_selector::is_matched (const abstract_svg_item *item) const 
{
  const svg_attribute_class *attribute_class = item->get_computed_attribute<svg_attribute_class> ();
  return attribute_class->is_class (m_class_name);
}
