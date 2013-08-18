#include "svg/attributes/svg_attribute_stroke_width.h"

svg_attribute_stroke_width::svg_attribute_stroke_width (svg_document *document)
  : abstract_attribute (document)
{
  m_stroke_width.set_value (1.0);
}

svg_attribute_stroke_width::~svg_attribute_stroke_width ()
{

}

bool svg_attribute_stroke_width::read (const char *data, bool from_css)
{
  return m_stroke_width.read (data, from_css);
}

bool svg_attribute_stroke_width::write (QString &data, bool from_css) const 
{
  data = m_stroke_width.write (from_css);
  return true;
}
