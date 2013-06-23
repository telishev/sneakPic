#include "svg/attributes/svg_attribute_id.h"

#include <QDomAttr>

svg_attribute_id::svg_attribute_id ()
{

}

svg_attribute_id::~svg_attribute_id ()
{

}

void svg_attribute_id::read (const QDomAttr &item)
{
  m_id = item.value ();
}

void svg_attribute_id::write (QDomAttr &item) const 
{
  item.setValue (m_id);
}
