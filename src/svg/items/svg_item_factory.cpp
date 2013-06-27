#include "svg/items/svg_item_factory.h"

#include "svg/items/svg_item_svg.h"
#include "svg/items/unknown_item.h"
#include "svg/items/svg_item_path.h"
#include "svg/items/svg_item_group.h"

template<typename T>
void svg_item_factory::support_item ()
{
  QString item_id = create_unique_item_name (T::static_name (), T::static_ns_URI ());
  m_map.insert (make_pair (item_id.toStdString (), [&] () { return new T (m_document); } ));
}

svg_item_factory::svg_item_factory (svg_document *document)
{
  m_document = document;

  support_item<svg_item_svg> ();
  support_item<svg_item_group> ();
  support_item<svg_item_path> ();
}

svg_item_factory::~svg_item_factory ()
{

}

abstract_svg_item *svg_item_factory::create_item (const QString &localName, const QString &namespaceURI, const QString &prefix) const
{
  QString item_id = create_unique_item_name (localName, namespaceURI);
  auto it = m_map.find (item_id.toStdString ());
  if (it == m_map.end ())
    return new unknown_item (m_document, localName, namespaceURI, prefix);

  return it->second ();
}

QString svg_item_factory::create_unique_item_name (const QString &localName, const QString &namespaceURI) const
{
  return namespaceURI + ":" + localName;
}

