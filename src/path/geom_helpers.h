#ifndef GEOM_HELPERS_H
#define GEOM_HELPERS_H

#include <QPointF>

#include "common/math_defs.h"

class QRect;
class QRectF;


namespace geom
{
  inline bool are_line (QPointF points[3])
  {

    return are_equal (points[0].x () * points[1].y () + points[1].x () * points[2].y () + points[2].x () * points[0].y (),
                      points[0].x () * points[2].y () + points[1].x () * points[0].y () + points[2].x () * points[1].y ());
  }

  inline bool are_line (QPointF first, QPointF second, QPointF third)
  {
    QPointF points[] = { first, second, third };
    return are_line (points);
  }


  QRect inner_rect (QRectF rect);
  QPoint nearest_point (QPointF point);
  double distance (QPointF first, QPointF second);
  double distance2 (QPointF first, QPointF second);
  double norm (QPointF point);
  double norm2 (QPointF point);
  QPointF normalized (QPointF point);
  QPointF direction (QPointF begin, QPointF end);
  double angle (QPointF point);
  double angle (QPointF begin, QPointF end);
  double dot (QPointF begin, QPointF end);
  double cross (QPointF begin, QPointF end);
  double angle_between (QPointF begin, QPointF end);
  QPointF vector_from_angle (double angle, double norm);
  QTransform rect2rect (const QRectF &src, const QRectF &dst);
  QPointF projection (QPointF start, QPointF end, QPointF point);
  double projection_value (QPointF start, QPointF end, QPointF point);
  QTransform triangle2triangle (const QPointF *source, const QPointF *dest);
  double rad2deg (double rad);
  double diagonal_length (QRectF rect);
  QRectF rect_from_2_points (QPointF point1, QPointF point2);
  void translate_by_qpoint (QTransform &transform, QPointF point);
  };

#endif // GEOM_HELPERS_H
