#include "svg/css/group_selector.h"

#include "common/memory_deallocation.h"

group_selector::group_selector ()
{

}

group_selector::~group_selector ()
{
  for (abstract_css_selector *selector : m_components)
    {
      FREE (selector);
    }
}

bool group_selector::is_matched (const abstract_svg_item *item) const 
{
  for (abstract_css_selector *selector : m_components)
    if (selector->is_matched (item))
      return true;

  return false;
}

std::string group_selector::to_string () const 
{
  std::string result;
  bool first = true;
  for (abstract_css_selector *selector : m_components)
    {
      if (!first)
        result += ',';

      first = false;
      result += selector->to_string ();
    }

  return result;
}
