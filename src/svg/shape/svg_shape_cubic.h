#ifndef SVG_SHAPE_CUBIC_H
#define SVG_SHAPE_CUBIC_H

#include "svg/shape/abstract_svg_shape.h"

class svg_shape_cubic : public abstract_svg_shape
{
  point_2d m_control_points[2];
public:
  svg_shape_cubic (const point_2d &start, const point_2d &stop, const point_2d &control1, const point_2d &control2);

  virtual ~svg_shape_cubic () {}

  point_2d control1 () const { return m_control_points[0]; }
  point_2d control2 () const { return m_control_points[1]; }

  virtual void add_to_path (QPainterPath &shape) override;
};

#endif // SVG_SHAPE_CUBIC_H
