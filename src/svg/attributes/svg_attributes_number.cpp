#include "svg/attributes/svg_attributes_number.h"

#include <QString>

#include "svg/items/abstract_svg_item.h"


bool svg_base_attribute_number::read (const QString &data, bool /*from_css*/ /*= false*/)
{
  bool ok;
  m_value = data.toDouble (&ok);
  return ok;
}

bool svg_base_attribute_number::write (QString &data, bool /*from_css*/ /*= false*/) const 
{
  data = QString::number (m_value);
  return true;
}

double svg_attribute_opacity::computed_opacity () const
{
  return m_value;
}
