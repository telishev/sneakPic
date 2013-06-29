#include "svg/items/svg_item_factory.h"

#include "svg/items/svg_item_svg.h"
#include "svg/items/unknown_item.h"
#include "svg/items/svg_item_path.h"
#include "svg/items/svg_item_group.h"
#include "svg/items/svg_item_style.h"
#include "svg/items/svg_item_defs.h"

#define DECLARE_ITEM(ENUM,NAME,NAMESPACE,CLASS)                                         \
svg_item_type CLASS::type () const { return svg_item_type::ENUM; }                      \
QString CLASS::static_name () { return NAME; }                                          \
svg_namespaces_t CLASS::static_ns_type () { return svg_namespaces_t::NAMESPACE; }       \
QString CLASS::static_ns_URI () { return svg_namespaces::uri (static_ns_type ()); }     \
QString CLASS::name () const { return static_name (); }                                 \
svg_namespaces_t CLASS::namespace_type () const { return static_ns_type (); }

  DECLARE_SVG_ITEM
#undef DECLARE_ITEM

template<typename T>
void svg_item_factory::support_item ()
{
  QString item_id = create_unique_item_name (T::static_name (), T::static_ns_URI ());
  m_map.insert (make_pair (item_id.toStdString (), [&] () { return new T (m_document); } ));
}

svg_item_factory::svg_item_factory (svg_document *document)
{
  m_document = document;

#define DECLARE_ITEM(ENUM,NAME,NAMESPACE,CLASS) support_item<CLASS> ();

  DECLARE_SVG_ITEM
#undef DECLARE_ITEM
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

