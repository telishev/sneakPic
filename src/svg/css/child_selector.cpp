#include "svg/css/child_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"

bool child_selector::is_matched (const abstract_svg_item *item) const 
{
  if (!m_rhs->is_matched (item))
    return false;

  abstract_svg_item *parent = item->parent ();
  if (!parent)
    return false;

  return m_lhs->is_matched (parent);
}
