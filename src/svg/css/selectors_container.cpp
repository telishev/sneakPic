#include "svg/css/selectors_container.h"

#include "common/debug_utils.h"

#include "svg/items/svg_item_style.h"

selectors_container::selectors_container ()
{

}

selectors_container::~selectors_container ()
{

}

void selectors_container::add_style (svg_item_style *style)
{
  DEBUG_ASSERT (m_style_set.find (style) == m_style_set.end ());
  m_style_set.insert (style);
}

void selectors_container::remove_style (svg_item_style *style)
{
  DEBUG_ASSERT (m_style_set.find (style) != m_style_set.end ());
  m_style_set.erase (style);
}

const abstract_attribute *selectors_container::get_attribute (const std::string &str, const abstract_svg_item *item_to_match) const
{
  /// TODO: in fact, we need to assemble all matching selectors,
  /// choose one with the most specificity or importance etc, but its too cumbersome to do right now
  for (svg_item_style *style : m_style_set)
    {
      const abstract_attribute *attribute = style->get_attribute (str, item_to_match);
      if (attribute)
        return attribute;
    }

  return nullptr;
}
