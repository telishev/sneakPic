#include "svg/attributes/svg_attribute_fill.h"

svg_attribute_fill::svg_attribute_fill (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_applied = true;
}

svg_attribute_fill::~svg_attribute_fill ()
{

}

bool svg_attribute_fill::read (const QString &data, bool /*from_css*/)
{
  if (data == "none")
    m_applied = false;
  else
    {
      m_applied = true;
      m_color = QColor (data);
    }

  return true;
}

bool svg_attribute_fill::write (QString &data, bool /*to_css*/) const 
{
  if (m_applied)
    data = QString ("#%1%2%3").arg (m_color.red ()).arg (m_color.green ()).arg (m_color.blue ());
  else
    data = "none";

  return true;
}
