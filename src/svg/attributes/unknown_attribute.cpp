#include "svg/attributes/unknown_attribute.h"
#include <QDomAttr>


void unknown_attribute::read (const QDomAttr &item)
{
  m_value = item.value ();
}

void unknown_attribute::write (QDomAttr &item) const 
{
  item.setValue (m_value);
}
