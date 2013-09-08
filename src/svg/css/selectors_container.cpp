#include "svg/css/selectors_container.h"

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"

#include "svg/css/css_declaration.h"
#include "svg/css/abstract_css_selector.h"

selectors_container::selectors_container ()
{

}

selectors_container::~selectors_container ()
{
  for (auto &rule : m_rule_set)
    {
      FREE (rule.first);
      FREE (rule.second);
    }
}

const abstract_attribute *selectors_container::get_attribute (const std::string &str, const abstract_svg_item *item_to_match) const
{
  for (auto rule : m_rule_set)
    {
      abstract_css_selector *selector = rule.first;
      css_declaration *declaration = rule.second;
      const abstract_attribute *attribute = declaration->get_attribute (str);
      if (!attribute)
        continue;

      /// TODO: in fact, we need to assemble all matching selectors,
      /// choose one with the most specificity or importance etc, but its too cumbersome to do right now
      if (selector->is_matched (item_to_match))
        return attribute;
    }

  return nullptr;
}

void selectors_container::add_rule_set (abstract_css_selector *selector, css_declaration *declaration)
{
  m_rule_set.push_back (std::make_pair (selector, declaration));
}

void selectors_container::get_attributes_for_item (const abstract_svg_item *item_to_match, std::unordered_map<std::string, abstract_attribute *> &result)
{
  for (auto rule : m_rule_set)
    {
      abstract_css_selector *selector = rule.first;
      css_declaration *declaration = rule.second;
      if (!selector->is_matched (item_to_match))
        continue;

      /// TODO: in fact, we need to assemble all matching selectors,
      /// choose one with the most specificity or importance etc, but its too cumbersome to do right now
      auto &attributes = declaration->attributes ();

      for (auto atr_pair : attributes)
        {
          result[atr_pair.first] = atr_pair.second;
        }
    }
}
