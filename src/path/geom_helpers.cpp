#include "geom_helpers.h"

#include <QRect>

QRect geom::inner_rect (QRectF rect)
{
  int left = (int)ceil (rect.left () + 0.5);
  int rigth = (int)floor (rect.right () - 0.5);
  int top = (int)ceil (rect.top () + 0.5);
  int bottom = (int)floor (rect.bottom () - 0.5);
  return QRect (left, top, rigth - left - 1, bottom - top - 1);
}

QPoint geom::nearest_point (QPointF point)
{
  return QPoint (qRound (point.x ()), qRound (point.y ()));
}

double geom::distance (QPoint first, QPoint second)
{
  return norm (first - second);
}

double geom::norm (QPoint point)
{
  return sqrt (point.x () * point.x () + point.y () * point.y ());
}
