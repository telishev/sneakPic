#include "svg/attributes/svg_attribute_points.h"

#include "common/common_utils.h"
#include "common/string_utils.h"

#include <QString>

svg_attribute_points::svg_attribute_points (svg_document *document)
  : abstract_attribute (document)
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
      // Actually error or warning should be printed in case of odd number of coordinates
      CHECK_RET (str_to_double (data, x), true);
      CHECK_RET (str_to_double (data, y), true);
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