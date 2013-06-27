#include "svg/shape/svg_shape_cubic.h"

#include <QPainterPath>

svg_shape_cubic::svg_shape_cubic (const point_2d &start, const point_2d &stop, const point_2d &control1, const point_2d &control2) : abstract_svg_shape (start, stop)
{
  m_control_points[0] = control1;
  m_control_points[1] = control2;
}

void svg_shape_cubic::add_to_path (QPainterPath &path)
{
  path.cubicTo (m_control_points[0].x (), m_control_points[0].y (), m_control_points[1].x (), m_control_points[1].y (),
                stop ().x (), stop ().y ());
}
