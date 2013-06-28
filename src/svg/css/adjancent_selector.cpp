#include "svg/css/adjancent_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"


bool adjancent_selector::is_matched (const abstract_svg_item *item) const 
{
  if (!m_rhs->is_matched (item))
    return false;

  abstract_svg_item *sibling = item->prev_sibling ();
  if (!sibling)
    return false;

  return m_lhs->is_matched (sibling);
}

