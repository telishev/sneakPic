#include "common/string_utils.h"
#include "svg/attributes/svg_attribute_points.h"

#include <QString>

svg_attribute_points::svg_attribute_points (abstract_svg_item *item)
  : abstract_attribute (item)
{
  
}

svg_attribute_points::~svg_attribute_points ()
{
}

bool svg_attribute_points::read (const char *data, bool /* from_css */)
{
  double x, y;
  while (*data)
    {
      CHECK (str_to_double (data, x));
      CHECK (str_to_double (data, y));
      m_point_list.push_back (QPointF (x, y));
    }
  return true;
}

bool svg_attribute_points::write (QString &data, bool /* from_css */) const
{
  for (int i = 0; i < m_point_list.size (); i++)
    {
      data = double_to_str (m_point_list[i].x ()) + "," + double_to_str (m_point_list[i].y ());
      if (i != m_point_list.size () - 1)
        data += " ";
    }
  return true;
}