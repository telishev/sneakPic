#include "svg/css/combinator_selector.h"

#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"


combinator_selector::combinator_selector (abstract_css_selector *lhs, abstract_css_selector *rhs)
{
  m_lhs = lhs;
  m_rhs = rhs;
}

combinator_selector::~combinator_selector ()
{
  FREE (m_lhs);
  FREE (m_rhs);
}

std::string combinator_selector::to_string () const
{
  return m_lhs->to_string () + combinator_str () + m_rhs->to_string ();
}



