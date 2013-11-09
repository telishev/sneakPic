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
  double distance (QPoint first, QPoint second);
  double norm (QPoint point);

};

#endif // GEOM_HELPERS_H
