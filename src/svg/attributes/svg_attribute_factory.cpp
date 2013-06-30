#include "svg/attributes/svg_attribute_factory.h"

#include "svg/attributes/unknown_attribute.h"
#include "svg/attributes/svg_attribute_id.h"
#include "svg/attributes/svg_attribute_version.h"
#include "svg/attributes/svg_attribute_width.h"
#include "svg/attributes/svg_attribute_height.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/svg_attribute_style.h"
#include "svg/attributes/svg_attribute_class.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attribute_stroke.h"
#include "svg/attributes/svg_attribute_fill.h"

#include <QString>

#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,STYLABLE)                                     \
  const char * CLASS::static_name () { return NAME; }                                             \
  svg_namespaces_t CLASS::static_ns_type () { return svg_namespaces_t::NAMESPACE; }               \
  svg_attribute_type CLASS::type () const { return svg_attribute_type::ENUM; }                    \
  const char * CLASS::static_ns_URI () { return svg_namespaces::uri (static_ns_type ()); }        \
  svg_namespaces_t CLASS::namespace_type () const { return static_ns_type (); }                   \
  const char *CLASS::namespace_uri () const { return svg_namespaces::uri (namespace_type ()); }   \
  const char *CLASS::namespace_name () const { return svg_namespaces::name (namespace_type ()); } \
  const char *CLASS::name () const { return static_name (); }                                     \
  bool CLASS::is_styleable () const { return static_is_styleable (); }                            \
  bool CLASS::static_is_styleable () { return STYLABLE; }                                         \
  const abstract_attribute *CLASS::default_value ()                                               \
 { static const CLASS def_value (nullptr); return &def_value; }                                   \
                                                                                                  
  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE

template<typename T>
void svg_attribute_factory::support_attribute ()
{
  QString item_id = create_unique_attribute_name (T::static_name (), T::static_ns_URI ());
  m_map.insert (make_pair (item_id.toStdString (), [] (abstract_svg_item *item) { return new T (item); } ));
}

svg_attribute_factory::svg_attribute_factory (svg_document *document)
{
  m_document = document;

#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,STYLABLE) support_attribute<CLASS> ();

  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE
}

svg_attribute_factory::~svg_attribute_factory ()
{

}

abstract_attribute *svg_attribute_factory::create_attribute (abstract_svg_item *item, const QString &localName, const QString &namespaceURI, const QString &prefix) const
{
  QString item_id = create_unique_attribute_name (localName, namespaceURI);
  auto it = m_map.find (item_id.toStdString ());
  if (it == m_map.end ())
    return new unknown_attribute (item, localName, namespaceURI, prefix);

  return it->second (item);
}

QString svg_attribute_factory::create_unique_attribute_name (const QString &localName, const QString &namespaceURI) const
{
  return namespaceURI + ":" + localName;
}

