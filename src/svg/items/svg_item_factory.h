#ifndef SVG_ITEM_FACTORY_H
#define SVG_ITEM_FACTORY_H

class abstract_svg_item;
class QString;
class svg_document;

enum class svg_item_type;

#include <functional>
#include <unordered_map>
#include <string>

class svg_item_factory
{
  svg_document *m_document;
  std::unordered_map<std::string, std::function<abstract_svg_item * ()>> m_string_map;
  std::unordered_map<svg_item_type, std::function<abstract_svg_item * ()>> m_type_map;
public:
  svg_item_factory (svg_document *document);
  ~svg_item_factory ();

  abstract_svg_item *create_item (const QString &localName, const QString &namespaceURI, const QString &prefix) const;
  abstract_svg_item *create_item (svg_item_type type);

private:
  template<typename T>
  void support_item ();

  QString create_unique_item_name (const QString &localName, const QString &namespaceURI) const;
};

#endif // SVG_ITEM_FACTORY_H
