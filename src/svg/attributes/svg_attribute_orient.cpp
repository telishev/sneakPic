#include "svg/attributes/svg_attribute_orient.h"

#include <QLineF>
#include <QPointF>

svg_attribute_orient::svg_attribute_orient ()
{
  m_angle.set_value (0.0);
  m_auto = true;
}

svg_attribute_orient::~svg_attribute_orient ()
{

}

bool svg_attribute_orient::read (const char *data, bool from_css)
{
  if (strcmp (data, "auto") == 0)
    return true;
  return m_angle.read (data, from_css);
}

double svg_attribute_orient::value (QPointF bisector) const
{  
  return m_auto ? QLineF (QPoint (), bisector).angle () : m_angle.value ();
}

bool svg_attribute_orient::write (QString &data, bool from_css) const 
{
  if (m_auto)
    {
      data = "auto";
      return true;
    }
  data = m_angle.write (from_css);
  return true;
}
