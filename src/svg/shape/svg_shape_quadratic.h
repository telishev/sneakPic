#ifndef SVG_SHAPE_QUADRATIC_H
#define SVG_SHAPE_QUADRATIC_H

#include "svg/shape/abstract_svg_shape.h"

class svg_shape_quadratic : abstract_svg_shape
{
  point_2d m_control_point;
public:
  svg_shape_quadratic (const point_2d &start, const point_2d &stop, const point_2d &control)
    : abstract_svg_shape (start, stop)
  {
    m_control_point = control;
  }

  virtual ~svg_shape_quadratic () {}
};

#endif // SVG_SHAPE_QUADRATIC_H
