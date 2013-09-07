#ifndef SVG_ATTRIBUTE_FACTORY_H
#define SVG_ATTRIBUTE_FACTORY_H

class abstract_attribute;
class svg_document;
class abstract_svg_item;

enum class svg_attribute_type;
enum class svg_item_type;

class QString;

#include <unordered_map>
#include <functional>
#include <string>

class svg_attribute_factory
{
  svg_document *m_document;
  std::unordered_multimap<std::string, std::function<abstract_attribute * ()>> m_string_map;
  std::unordered_map<svg_attribute_type, std::function<abstract_attribute * ()>> m_type_map;
public:
  svg_attribute_factory (svg_document *document);
  ~svg_attribute_factory ();

  abstract_attribute *create_attribute (abstract_svg_item *item, const char *localName, const char *namespaceURI, const char *prefix) const;
  abstract_attribute *create_attribute (const char *localName, svg_item_type type) const;
  abstract_attribute *create_attribute (int item_id, svg_attribute_type type);

private:
  template<typename T>
  void support_attribute ();
};

#endif // SVG_ATTRIBUTE_FACTORY_H
