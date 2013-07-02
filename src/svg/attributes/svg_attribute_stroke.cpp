#include "svg/attributes/svg_attribute_stroke.h"

svg_attribute_stroke::svg_attribute_stroke (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_applied = false;
}

svg_attribute_stroke::~svg_attribute_stroke ()
{

}

bool svg_attribute_stroke::read (const QString &data, bool /*from_css*/)
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

bool svg_attribute_stroke::write (QString &data, bool /*to_css*/) const 
{
  if (m_applied)
    data = QString ("#%1%2%3").arg (m_color.red ()).arg (m_color.green ()).arg (m_color.blue ());
  else
    data = "none";

  return true;
}
