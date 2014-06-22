#include "bezier_helpers.h"

#include <QPointF>

#include "common/math_defs.h"
#include "path/single_path_point.h"


QPointF bezier_helpers::get_pos_by_value (const single_path_segment &segment, double t) const
{
  double t1 = 1.0 - t;
  return   cubed (t) * segment.end + 3 * t1 * squared (t) * segment.c2
         + 3 * t * squared (t1) * segment.c1 + cubed (t1) * segment.start;
}

QPointF bezier_helpers::get_tangent_by_value (const single_path_segment &segment, double t) const
{
  double t1 = 1.0 - t;
  return   3 * squared (t1) * (segment.start - segment.c1) + 6 * t1 * t * (segment.c2 - segment.c1)
         + 3 * squared (t) * (segment.end - segment.c2);
}

std::vector<single_path_segment> bezier_helpers::split (const single_path_segment &segment, double t) const
{
  QPointF p1 = segment.start;
  QPointF p2 = segment.c1;
  QPointF p3 = segment.c2;
  QPointF p4 = segment.end;

  QPointF p12 = (p2 - p1) * t + p1;
  QPointF p23 = (p3 - p2) * t + p2;
  QPointF p34 = (p4 - p3) * t + p3;

  QPointF p123 = (p23 - p12) * t + p12;
  QPointF p234 = (p34 - p23) * t + p23;
  QPointF p1234 = (p234 - p123) * t + p123;

  return {{p1, p1234, p12, p123}, {p1234, p4, p234, p34}};
}
