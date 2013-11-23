#include "svg/attributes/svg_attribute_factory.h"

#include "svg/attributes/svg_attribute_element_mapping.h"

#include "svg/items/abstract_svg_item.h"

#include "svg/attributes/svg_attribute_type.h"
#include "svg/svg_namespaces.h"

#include "svg/attributes/svg_attribute_id.h"
#include "svg/attributes/svg_attribute_version.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/svg_attribute_style.h"
#include "svg/attributes/svg_attribute_class.h"
#include "svg/attributes/svg_attribute_stroke_linecap.h"
#include "svg/attributes/svg_attribute_stroke_linejoin.h"
#include "svg/attributes/svg_attribute_stroke_miterlimit.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attributes_length_type_list.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attribute_gradient_transform.h"
#include "svg/attributes/svg_attribute_stop_color.h"
#include "svg/attributes/svg_attribute_offset.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/attributes/svg_attribute_clip_path.h"
#include "svg/attributes/svg_attribute_viewbox.h"
#include "svg/attributes/svg_attributes_string.h"
#include "svg/attributes/svg_attribute_font_size.h"
#include "svg/attributes/svg_attribute_points.h"
#include "svg/attributes/svg_attribute_color.h"
#include "svg/attributes/svg_attribute_preserve_aspect_ratio.h"
#include "svg/attributes/svg_attribute_orient.h"
#include "svg/attributes/svg_attributes_marker_usage.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "svg/attributes/svg_attribute_linetypes.h"

#include <QString>
#include <memory>


#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,INHERIT_TYPE)                                  \
  const char * CLASS::static_type_name () { return NAME; }                                         \
  svg_namespaces_t CLASS::static_ns_type () { return svg_namespaces_t::NAMESPACE; }                \
  svg_attribute_type CLASS::static_type () { return svg_attribute_type::ENUM; }                    \
  svg_attribute_type CLASS::type () const { return svg_attribute_type::ENUM; }                     \
  const char * CLASS::static_ns_URI () { return svg_namespaces::uri (static_ns_type ()); }         \
  svg_namespaces_t CLASS::namespace_type () const { return static_ns_type (); }                    \
  const char *CLASS::namespace_uri () const { return svg_namespaces::uri (namespace_type ()); }    \
  const char *CLASS::namespace_name () const { return svg_namespaces::name (namespace_type ()); }  \
  const char *CLASS::type_name () const { return static_type_name (); }                            \
  svg_inherit_type CLASS::inherit_type () const { return static_inherit_type (); }                 \
  svg_inherit_type CLASS::static_inherit_type () { return svg_inherit_type::INHERIT_TYPE; }        \
  const abstract_attribute *CLASS::default_value ()                                                \
 { static const CLASS def_value; return &def_value; }                                              \
                                                                                                  
  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE

template<typename T>
void svg_attribute_factory::support_attribute ()
{
  std::string type_name = T::static_type_name ();
  svg_attribute_type type = T::static_type ();
  m_string_map.insert (std::make_pair (type_name, [&] () { return new T (); } ));
  m_type_map.insert (std::make_pair (type, [&] () { return new T (); } ));
  m_type_to_string_map[type] = type_name;
}

svg_attribute_factory::svg_attribute_factory ()
{
#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,STYLABLE) support_attribute<CLASS> ();

  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE
}

svg_attribute_factory::~svg_attribute_factory ()
{

}

abstract_attribute *svg_attribute_factory::create_attribute (abstract_svg_item *item, const char *localName, const char *namespaceURI) const
{
  std::string item_id = localName;
  auto it_pair = m_string_map.equal_range (item_id);
  const svg_attribute_element_mapping *mapping = svg_attribute_element_mapping::get ();

  for (auto it = it_pair.first; it != it_pair.second; ++it)
    {
      std::unique_ptr<abstract_attribute> attribute (it->second ());
      if (*namespaceURI && strcmp (attribute->namespace_uri (), namespaceURI))
        continue;

      if (!mapping->can_be_specified (item->type (), attribute->type ()))
        continue;

      attribute->set_item (item->undo_id ());
      return attribute.release ();
    }

  return nullptr;
}

abstract_attribute *svg_attribute_factory::create_attribute (int item_id, svg_attribute_type type)
{
  abstract_attribute *attribute = m_type_map[type] ();
  attribute->set_item (item_id);
  return attribute;
}

abstract_attribute * svg_attribute_factory::create_attribute (const char *localName, svg_item_type type) const
{
  std::string item_id = localName;
  auto it_pair = m_string_map.equal_range (item_id);
  const svg_attribute_element_mapping *mapping = svg_attribute_element_mapping::get ();

  for (auto it = it_pair.first; it != it_pair.second; ++it)
    {
      std::unique_ptr<abstract_attribute> attribute (it->second ());
      if (!mapping->can_be_specified (type, attribute->type ()))
        continue;

      return attribute.release ();
    }

  return nullptr;
}

svg_attribute_factory *svg_attribute_factory::get ()
{
  static svg_attribute_factory instance;
  return &instance;
}

std::string svg_attribute_factory::attribute_name (svg_attribute_type type) const
{
  return m_type_to_string_map.at (type);
}
