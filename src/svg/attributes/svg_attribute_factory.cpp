#include "svg/attributes/svg_attribute_factory.h"

#include <QString>

#include "svg/attributes/unknown_attribute.h"
#include "svg/attributes/svg_attribute_id.h"

template<typename T>
void svg_attribute_factory::support_attribute ()
{
  QString item_id = create_unique_attribute_name (T::static_name (), T::static_ns_URI ());
  m_map.insert (make_pair (item_id.toStdString (), [] () { return new T (); } ));
}

svg_attribute_factory::svg_attribute_factory (svg_document *document)
{
  m_document = document;

  support_attribute<svg_attribute_id> ();
}

svg_attribute_factory::~svg_attribute_factory ()
{

}

abstract_attribute *svg_attribute_factory::create_attribute (const QString &localName, const QString &namespaceURI, const QString &prefix) const
{
  QString item_id = create_unique_attribute_name (localName, namespaceURI);
  auto it = m_map.find (item_id.toStdString ());
  if (it == m_map.end ())
    return new unknown_attribute (localName, namespaceURI, prefix);

  return it->second ();
}

QString svg_attribute_factory::create_unique_attribute_name (const QString &localName, const QString &namespaceURI) const
{
  return namespaceURI + ":" + localName;
}
