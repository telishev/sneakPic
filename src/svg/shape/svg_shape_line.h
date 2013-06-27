#ifndef SVG_SHAPE_LINE_H
#define SVG_SHAPE_LINE_H

#include "svg/shape/abstract_svg_shape.h"

class svg_shape_line : public abstract_svg_shape
{
public:
  svg_shape_line (const point_2d &start, const point_2d &stop);

  virtual ~svg_shape_line () {}

  virtual void add_to_path (QPainterPath &shape) override;
};

#endif // SVG_SHAPE_LINE_H
