#include "svg/attributes/svg_attribute_stroke_width.h"

svg_attribute_stroke_width::svg_attribute_stroke_width (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_stroke_width.set_value (1.0);
}

svg_attribute_stroke_width::~svg_attribute_stroke_width ()
{

}

bool svg_attribute_stroke_width::read (const QString &data, bool from_css)
{
  return m_stroke_width.read (data.toUtf8 ().constData (), from_css);
}

bool svg_attribute_stroke_width::write (QString &data, bool from_css) const 
{
  data = m_stroke_width.write (from_css);
  return true;
}
