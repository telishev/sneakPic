#include "svg/data_types/data_type_iri.h"

#include "svg/svg_document.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"

data_type_iri::data_type_iri (abstract_svg_item *item)
{
  m_item = item;
}

data_type_iri::~data_type_iri ()
{

}

bool data_type_iri::read (const QString &data)
{
  /// For now we only support local references in a form  #<elementID>
  if (!data.startsWith ("#"))
    return false;

  m_element_id = data.mid (1);
  return true;
}

bool data_type_iri::write (QString &data) const 
{
  data = QLatin1String ("#") + m_element_id;
  return true;
}

abstract_svg_item *data_type_iri::href () const
{
  if (!m_item)
    return nullptr;

  return m_item->document ()->item_container ()->get_item (m_element_id);
}


