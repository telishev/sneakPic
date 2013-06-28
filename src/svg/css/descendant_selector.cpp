#include "svg/css/descendant_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"

bool descendant_selector::is_matched (const abstract_svg_item *item) const 
{
  if (!m_rhs->is_matched (item))
    return false;

  for (abstract_svg_item *parent = item->parent (); parent; parent = parent->parent ())
    if (m_lhs->is_matched (parent))
      return true;

  return false;
}