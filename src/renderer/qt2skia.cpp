#include "renderer/qt2skia.h"

#include <QTransform>
#include <QPointF>
#include <QPainterPath>
#include <QBrush>

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#include <SkPoint.h>
#pragma warning(pop)

SkMatrix qt2skia::matrix (const QTransform &tr)
{
  SkMatrix matrix;
  matrix.setAll (tr.m11 (), tr.m21 (), tr.m31 (), tr.m12 (), tr.m22 (), tr.m32 (), tr.m13 (), tr.m23 (), tr.m33 ());
  return matrix;
}

SkPoint qt2skia::point (const QPointF &point)
{
  return SkPoint::Make (SkFloatToScalar (point.x ()), SkFloatToScalar (point.y ()));
}

SkPath qt2skia::path (const QPainterPath &qpath)
{
  SkPath path;
  for (int i = 0; i < qpath.elementCount (); i++)
    {
      QPainterPath::Element elem = qpath.elementAt (i);
      switch  (elem.type)
        {
        case QPainterPath::MoveToElement:
          path.moveTo (qt2skia::point (elem));
          break;
        case QPainterPath::LineToElement:
          path.lineTo (qt2skia::point (elem));
          break;
        case QPainterPath::CurveToElement:
          {
            QPainterPath::Element control2 = qpath.elementAt (++i);
            QPainterPath::Element end = qpath.elementAt (++i);
            path.cubicTo (qt2skia::point (elem), qt2skia::point (control2), qt2skia::point (end));
            break;
          }
        default:
          break;
        }
    }
  return path;
}

SkColor qt2skia::color (const QColor &color)
{
  return SkColorSetARGBInline (color.alpha (), color.red (), color.green (), color.blue ());
}

QImage qt2skia::qimage (const SkBitmap &bitmap)
{
  SkAutoLockPixels image_lock (bitmap);
  return QImage ((unsigned char*)bitmap.getPixels (), bitmap.width (), bitmap.height (), (int)bitmap.rowBytes (), QImage::Format_ARGB32_Premultiplied);
}
