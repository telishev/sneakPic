#include "svg/attributes/svg_attribute_height.h"

#include <QString>

svg_attribute_height::svg_attribute_height (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_height.set_value (297, svg_length_units::MM); /// A4
}

svg_attribute_height::~svg_attribute_height ()
{

}

bool svg_attribute_height::read (const QString &data, bool from_css)
{
  return m_height.read (data.toUtf8 ().constData (), from_css);
}

bool svg_attribute_height::write (QString &data, bool from_css) const 
{
  data = m_height.write (from_css);
  return true;
}

double svg_attribute_height::height () const
{
  return m_height.value ();
}


