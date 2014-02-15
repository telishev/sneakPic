#ifndef SVG_ATTRIBUTE_FACTORY_H
#define SVG_ATTRIBUTE_FACTORY_H

class abstract_attribute;
class abstract_svg_item;

enum class svg_attribute_type;
enum class svg_item_type;

class QString;

#include <map>
#include <unordered_map>
#include <functional>
#include <string>

class svg_attribute_factory
{
  std::unordered_multimap<std::string, std::function<abstract_attribute * ()>> m_string_map;
  map<svg_attribute_type, std::function<abstract_attribute * ()>> m_type_map;
  map<svg_attribute_type, std::string> m_type_to_string_map;
public:
  static svg_attribute_factory *get ();

  abstract_attribute *create_attribute (abstract_svg_item *item, const char *localName, const char *namespaceURI) const;
  abstract_attribute *create_attribute (const char *localName, svg_item_type type) const;
  abstract_attribute *create_attribute (int item_id, svg_attribute_type type);

  std::string attribute_name (svg_attribute_type type) const;

private:
  svg_attribute_factory ();
  ~svg_attribute_factory ();

  template<typename T>
  void support_attribute ();
};

#endif // SVG_ATTRIBUTE_FACTORY_H
