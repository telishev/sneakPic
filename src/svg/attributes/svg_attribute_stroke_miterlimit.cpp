#include "svg/attributes/svg_attribute_stroke_miterlimit.h"

svg_attribute_stroke_miterlimit::svg_attribute_stroke_miterlimit (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_miterlimit = 4.0;
}

svg_attribute_stroke_miterlimit::~svg_attribute_stroke_miterlimit ()
{
}

bool svg_attribute_stroke_miterlimit::read (const QString &data, bool /*from_css*/)
{
  bool ok;
  m_miterlimit = data.toDouble (&ok);
  if (!ok || m_miterlimit < 1.0) // According to svg specification mitterlimit < 1.0 should cause an error, for now just defaulting it
    m_miterlimit = 4.0;
  return true;
}

bool svg_attribute_stroke_miterlimit::write (QString &data, bool /*from_css*/) const
{
  data = QString::number (m_miterlimit);
  return true;
}