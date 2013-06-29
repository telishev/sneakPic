#include "svg/attributes/svg_attribute_height.h"

svg_attribute_height::svg_attribute_height (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_height = 0;
}

svg_attribute_height::~svg_attribute_height ()
{

}

bool svg_attribute_height::read (const QString &data, bool /*from_css*/)
{
  bool ok; m_height = data.toDouble (&ok); return ok;
}

bool svg_attribute_height::write (QString &data, bool /*to_css*/) const 
{
  data = QString::number (m_height) ; return true;
}


