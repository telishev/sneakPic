#include "svg/attributes/svg_attribute_stroke_width.h"

svg_attribute_stroke_width::svg_attribute_stroke_width (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_stroke_width = 1.0;
}

svg_attribute_stroke_width::~svg_attribute_stroke_width ()
{

}

bool svg_attribute_stroke_width::read (const QString &data, bool /*from_css*/)
{
  bool ok;
  m_stroke_width = data.toDouble (&ok);
  return ok;
}

bool svg_attribute_stroke_width::write (QString &data, bool /*to_css*/) const 
{
  data = QString::number (m_stroke_width);
  return true;
}
