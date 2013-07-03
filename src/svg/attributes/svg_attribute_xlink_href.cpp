#include "svg/attributes/svg_attribute_xlink_href.h"

#include "svg/svg_document.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"

svg_attribute_xlink_href::svg_attribute_xlink_href (abstract_svg_item *item)
  : abstract_attribute (item), m_iri (item)
{

}

svg_attribute_xlink_href::~svg_attribute_xlink_href ()
{

}

bool svg_attribute_xlink_href::read (const QString &data, bool /*from_css*/)
{
  return m_iri.read (data);
}

bool svg_attribute_xlink_href::write (QString &data, bool /*to_css*/) const 
{
  return m_iri.write (data);
}

abstract_svg_item *svg_attribute_xlink_href::href () const
{
  return m_iri.href ();
}
