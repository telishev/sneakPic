#include "svg/css/adjancent_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"


bool adjancent_selector::is_matched (const abstract_svg_item *item) const 
{
  if (!m_rhs->is_matched (item))
    return false;

  int item_child_index = item->child_index ();
  int sibling_index = item_child_index - 1;

  if (sibling_index < 0)
    return false;

  abstract_svg_item *sibling = item->parent ()->child (sibling_index);
  if (!sibling)
    return false;

  return m_lhs->is_matched (sibling);
}

