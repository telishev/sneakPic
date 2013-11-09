#include "geom_helpers.h"

#include <QRect>

QRect geom_helpers::inner_rect (QRectF rect)
{
  int left = (int)ceil (rect.left () + 0.5);
  int rigth = (int)floor (rect.right () - 0.5);
  int top = (int)ceil (rect.top () + 0.5);
  int bottom = (int)floor (rect.bottom () - 0.5);
  return QRect (left, top, rigth - left - 1, bottom - top - 1);
}
