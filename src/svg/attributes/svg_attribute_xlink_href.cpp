#include "svg/attributes/svg_attribute_xlink_href.h"

#include "svg/svg_document.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"

svg_attribute_xlink_href::svg_attribute_xlink_href (abstract_svg_item *item)
  : abstract_attribute (item)
{

}

svg_attribute_xlink_href::~svg_attribute_xlink_href ()
{

}

bool svg_attribute_xlink_href::read (const QString &data, bool /*from_css*/)
{
  /// For now we only support local references in a form  #<elementID>
  if (!data.startsWith ("#"))
    return false;

  m_element_id = data.mid (1);
  return true;
}

bool svg_attribute_xlink_href::write (QString &data, bool /*to_css*/) const 
{
  data = QLatin1String ("#") + m_element_id;
  return true;
}

abstract_svg_item *svg_attribute_xlink_href::href () const
{
  if (!item ())
    return nullptr;

  return item ()->document ()->item_container ()->get_item (m_element_id);
}
