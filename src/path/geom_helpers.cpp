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

double geom::distance2 (QPointF first, QPointF second)
{
  return norm2 (first - second);
}

double geom::norm (QPointF point)
{
  return sqrt (point.x () * point.x () + point.y () * point.y ());
}

double geom::norm2 (QPointF point)
{
  return point.x () * point.x () + point.y () * point.y ();
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

double geom::projection_value (QPointF v, QPointF w, QPointF p)
{
  // Return minimum distance between line segment vw and point p
  double l2 = distance2 (v, w);  // i.e. |w-v|^2 -  avoid a sqrt
  if (l2 < MINIMAL_FOR_COMPARISON) return 0.0;   // v == w case
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line. 
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  double t = dot (p - v, w - v) / l2;
  if (t < 0.0) return 0.0;       // Beyond the 'v' end of the segment
  else if (t > 1.0) return 1.0;  // Beyond the 'w' end of the segment
  return t;
}

QPointF geom::projection (QPointF v, QPointF w, QPointF p)
{
  double value = projection_value (v, w, p);
  return v * (1.0 - value) + w * value;
}

QTransform geom::triangle2triangle (const QPointF *source, const QPointF *dest)
{
  double x11 = source[0].x ();
  double x12 = source[0].y ();
  double x21 = source[1].x ();
  double x22 = source[1].y ();
  double x31 = source[2].x ();
  double x32 = source[2].y ();
  double y11 = dest[0].x ();
  double y12 = dest[0].y ();
  double y21 = dest[1].x ();
  double y22 = dest[1].y ();
  double y31 = dest[2].x ();
  double y32 = dest[2].y ();

  double a1 = ((y11-y21)*(x12-x32)-(y11-y31)*(x12-x22))/
              ((x11-x21)*(x12-x32)-(x11-x31)*(x12-x22));
  double a2 = ((y11-y21)*(x11-x31)-(y11-y31)*(x11-x21))/
              ((x12-x22)*(x11-x31)-(x12-x32)*(x11-x21));
  double a3 = y11-a1*x11-a2*x12;
  double a4 = ((y12-y22)*(x12-x32)-(y12-y32)*(x12-x22))/
              ((x11-x21)*(x12-x32)-(x11-x31)*(x12-x22));
  double a5 = ((y12-y22)*(x11-x31)-(y12-y32)*(x11-x21))/
              ((x12-x22)*(x11-x31)-(x12-x32)*(x11-x21));
  double a6 = y12-a4*x11-a5*x12;

  return QTransform (a1, a4, a2, a5, a3, a6);
}

