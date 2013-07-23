#ifndef SVG_ATTRIBUTE_ELEMENT_MAPPING_H
#define SVG_ATTRIBUTE_ELEMENT_MAPPING_H

#include <unordered_set>
#include <vector>

enum class svg_item_type;
enum class svg_attribute_type;

class svg_attribute_element_mapping
{
  std::vector<std::unordered_set<svg_item_type>> m_map;
public:
  static svg_attribute_element_mapping *get ();

  bool can_be_specified (svg_item_type item_type, svg_attribute_type attribute_type) const;

private:
  svg_attribute_element_mapping ();
  ~svg_attribute_element_mapping ();

  void fill_presentation_attribute (svg_attribute_type type);
  void fill_all_items (svg_attribute_type type);
  void check ();
};

#endif // SVG_ATTRIBUTE_ELEMENT_MAPPING_H
