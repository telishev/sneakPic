#include "svg/items/svg_item_factory.h"

#include "svg/items/svg_item_svg.h"
#include "svg/items/unknown_item.h"
#include "svg/items/svg_item_path.h"
#include "svg/items/svg_item_group.h"
#include "svg/items/svg_item_style.h"
#include "svg/items/svg_item_defs.h"
#include "svg/items/svg_item_type.h"
#include "svg/items/svg_item_image.h"
#include "svg/items/svg_item_line.h"
#include "svg/items/svg_item_rect.h"
#include "svg/items/svg_item_circle.h"
#include "svg/items/svg_item_ellipse.h"
#include "svg/items/svg_item_use.h"
#include "svg/items/svg_item_stop.h"
#include "svg/items/svg_item_linear_gradient.h"
#include "svg/items/svg_item_radial_gradient.h"
#include "svg/items/svg_item_clip_path.h"
#include "svg/items/svg_item_text.h"
#include "svg/items/svg_item_polygon.h"
#include "svg/items/svg_item_polyline.h"
#include "svg/items/svg_item_a.h"

#include "svg/svg_namespaces.h"

#define DECLARE_ITEM(ENUM,NAME,NAMESPACE,CLASS)                                         \
svg_item_type CLASS::static_type () { return svg_item_type::ENUM; };                    \
svg_item_type CLASS::type () const { return svg_item_type::ENUM; }                      \
const char *CLASS::static_type_name () { return NAME; }                                 \
svg_namespaces_t CLASS::static_ns_type () { return svg_namespaces_t::NAMESPACE; }       \
const char *CLASS::static_ns_URI () { return svg_namespaces::uri (static_ns_type ()); } \
const char *CLASS::type_name () const { return static_type_name (); }                   \
svg_namespaces_t CLASS::namespace_type () const { return static_ns_type (); }

  DECLARE_SVG_ITEM
#undef DECLARE_ITEM

template<typename T>
void svg_item_factory::support_item ()
{
  QString item_id = create_unique_item_name (T::static_type_name (), T::static_ns_URI ());
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

