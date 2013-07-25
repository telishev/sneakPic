#ifndef SELECTORS_CONTAINER_H
#define SELECTORS_CONTAINER_H

#include <unordered_set>
#include <string>

class svg_item_style;
class abstract_svg_item;
class abstract_attribute;

class selectors_container
{
  std::unordered_set<svg_item_style *> m_style_set;

public:
  selectors_container ();
  ~selectors_container ();

  void add_style (svg_item_style *style);
  void remove_style (svg_item_style *style);

  const abstract_attribute *get_attribute (const std::string &str, const abstract_svg_item *item_to_match) const;
};

#endif // SELECTORS_CONTAINER_H
