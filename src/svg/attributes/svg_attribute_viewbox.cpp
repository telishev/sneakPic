#include "common/common_utils.h"
#include "common/string_utils.h"

#include "svg/attributes/svg_attribute_viewbox.h"

#include <QTransform>
#include <QRectF>
#include "path/geom_helpers.h"


svg_attribute_view_box::svg_attribute_view_box ()
{
  m_x = 0.0;
  m_y = 0.0;
  m_width = 0.0;
  m_height = 0.0;
}

svg_attribute_view_box::~svg_attribute_view_box ()
{

}

bool svg_attribute_view_box::read (const char *data, bool /*from_css*/)
{
  CHECK (str_to_double (data, m_x));
  CHECK (str_to_double (data, m_y));
  CHECK (str_to_double (data, m_width));
  CHECK (str_to_double (data, m_height));
  return true;
}

bool svg_attribute_view_box::write (QString &data, bool /*to_css*/) const 
{
  QString x = double_to_str (m_x), y = double_to_str (m_y), width = double_to_str (m_width), height = double_to_str (m_height);
  data = QString ("%1 %2 %3 %4").arg (x, y, width, height);
  return true;
}

QTransform svg_attribute_view_box::get_transform (const QRectF &viewport) const
{
  /// TODO: add support for preserveAspectRatio
  QRectF src = QRectF (m_x, m_y, m_width, m_height);
  return geom::rect2rect (src, viewport);
}
