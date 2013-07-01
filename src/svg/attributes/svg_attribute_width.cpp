#include "svg_attribute_width.h"

svg_attribute_width::svg_attribute_width (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_width.set_value (210, svg_length_units::MM); /// A4
}

svg_attribute_width::~svg_attribute_width ()
{

}

double svg_attribute_width::width () const
{
  return m_width.value ();
}

bool svg_attribute_width::read (const QString &data, bool from_css)
{
  return m_width.read (data.toUtf8 ().constData (), from_css);
}

bool svg_attribute_width::write (QString &data, bool from_css) const 
{
  data = m_width.write (from_css);
  return true;
}
