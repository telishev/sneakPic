#ifndef SVG_SHAPE_ELLIPTIC_ARC_H
#define SVG_SHAPE_ELLIPTIC_ARC_H

#include "svg/shape/abstract_svg_shape.h"

class svg_shape_elliptic_arc : abstract_svg_shape
{
  double rx, ry;
  double rotation;
  bool large_arc_flag;
  bool sweep_flag;
public:
  svg_shape_elliptic_arc ();
  virtual ~svg_shape_elliptic_arc ();
};
#endif // SVG_SHAPE_ELLIPTIC_ARC_H

