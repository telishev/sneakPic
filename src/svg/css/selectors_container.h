#ifndef SELECTORS_CONTAINER_H
#define SELECTORS_CONTAINER_H

#include <unordered_map>
#include <string>
#include <vector>

class abstract_css_selector;
class css_declaration;
class abstract_svg_item;
class abstract_attribute;

class selectors_container
{
  vector<std::pair <abstract_css_selector *, css_declaration *>> m_rule_set;

public:
  selectors_container ();
  ~selectors_container ();

  bool empty () const { return m_rule_set.empty (); }
  void add_rule_set (abstract_css_selector *selector, css_declaration *declaration);

  const abstract_attribute *get_attribute (const string &str, const abstract_svg_item *item_to_match) const;
  void get_attributes_for_item (const abstract_svg_item *item_to_match, std::unordered_map<string, abstract_attribute *> &result);
};

#endif // SELECTORS_CONTAINER_H
