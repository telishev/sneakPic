#include "geom_helpers.h"

#include <QRect>
#include <QTransform>
#include <math.h>

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

double geom::distance (QPointF first, QPointF second)
{
  return norm (first - second);
}

double geom::norm (QPointF point)
{
  return sqrt (point.x () * point.x () + point.y () * point.y ());
}

QPointF geom::normalized (QPointF point)
{
  return point / norm (point);
}

QPointF geom::direction (QPointF begin, QPointF end)
{
  return normalized (end - begin);
}

double geom::angle (QPointF point)
{
  return atan2 (point.y (), point.x ());
}

double geom::angle (QPointF begin, QPointF end)
{
  return angle (end - begin);
}

QPointF geom::vector_from_angle (double angle, double norm)
{
  return norm * QPointF (cos (angle), sin (angle));
}

double geom::dot (QPointF begin, QPointF end)
{
  return begin.x () * end.x () + begin.y () * end.y ();
}

double geom::cross (QPointF begin, QPointF end)
{
  return begin.x () * end.y () - begin.y () * end.x ();
}

double geom::angle_between (QPointF begin, QPointF end)
{
  return atan2 (cross (end, begin), dot (end, begin));
}

QTransform geom::rect2rect (const QRectF &src, const QRectF &dst)
{
  double m11 = dst.width () / src.width ();
  double m22 = dst.height () / src.height ();
  double dx = dst.x () - src.x () * m11;
  double dy = dst.y () - src.y () * m22;
  return QTransform (m11, 0.0, 0.0, m22, dx, dy);
}
