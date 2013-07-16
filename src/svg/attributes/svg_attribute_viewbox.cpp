#include "svg/attributes/svg_attribute_viewbox.h"

#include <QTransform>
#include <QRectF>

#include "common/string_utils.h"


svg_attribute_view_box::svg_attribute_view_box (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_x = 0.0;
  m_y = 0.0;
  m_width = 0.0;
  m_height = 0.0;
}

svg_attribute_view_box::~svg_attribute_view_box ()
{

}

bool svg_attribute_view_box::read (const QString &data, bool /*from_css*/)
{
  QByteArray arr = data.toUtf8 ();
  const char *char_data = arr.constData ();
  CHECK (str_to_double (char_data, m_x));
  CHECK (str_to_double (char_data, m_y));
  CHECK (str_to_double (char_data, m_width));
  CHECK (str_to_double (char_data, m_height));
  return true;
}

bool svg_attribute_view_box::write (QString &data, bool /*to_css*/) const 
{
  QString x = double_to_str (m_x), y = double_to_str (m_y), width = double_to_str (m_width), heigth = double_to_str (m_height);
  data = QString ("%1 %2 %3 %4").arg (m_x, m_y, m_width, m_height);
  return true;
}

static QTransform rect2rect (const QRectF &src, const QRectF &dst)
{
  double m11 = dst.width () / src.width ();
  double m22 = dst.height () / src.height ();
  double dx = dst.x () - src.x () * m11;
  double dy = dst.y () - src.y () * m22;
  return QTransform (m11, 0.0, 0.0, m22, dx, dy);
}

QTransform svg_attribute_view_box::get_transform (const QRectF &viewport) const
{
  /// TODO: add support for preserveAspectRatio
  QRectF src = QRectF (m_x, m_y, m_width, m_height);
  return rect2rect (src, viewport);
}
