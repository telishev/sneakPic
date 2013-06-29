#ifndef SVG_ATTRIBUTE_FACTORY_H
#define SVG_ATTRIBUTE_FACTORY_H

class abstract_attribute;
class svg_document;
class abstract_svg_item;

class QString;

#include <unordered_map>
#include <functional>
#include <string>

class svg_attribute_factory
{
  svg_document *m_document;
  std::unordered_map<std::string, std::function<abstract_attribute * (abstract_svg_item *)>> m_map;
public:
  svg_attribute_factory (svg_document *document);
  ~svg_attribute_factory ();

  abstract_attribute *create_attribute (abstract_svg_item *item, const QString &localName, const QString &namespaceURI, const QString &prefix) const;

private:
  template<typename T>
  void support_attribute ();

  QString create_unique_attribute_name (const QString &localName, const QString &namespaceURI) const;
};

#endif // SVG_ATTRIBUTE_FACTORY_H
