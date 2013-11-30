#include "common/math_defs.h"

#include "renderer/qt2skia.h"

#include <QTransform>
#include <QPointF>
#include <QPainterPath>
#include <QBrush>
#include <QRect>
#include <QRectF>

#include "path/svg_path.h"
#include "skia/skia_includes.h"
namespace qt2skia {

SkMatrix matrix (const QTransform &tr)
{
  SkMatrix matrix;
  matrix.setAll (tr.m11 (), tr.m21 (), tr.m31 (), tr.m12 (), tr.m22 (), tr.m32 (), tr.m13 (), tr.m23 (), tr.m33 ());
  return matrix;
}

SkPoint point (const QPointF &point)
{
  return SkPoint::Make (SkFloatToScalar (point.x ()), SkFloatToScalar (point.y ()));
}

int fill_rule (int rule)
{
  switch (rule)
    {
    case Qt::FillRule::OddEvenFill:
      return SkPath::FillType::kEvenOdd_FillType;
    case Qt::FillRule::WindingFill:
      return SkPath::FillType::kWinding_FillType;
    }
  return SkPath::FillType::kWinding_FillType;
}

SkPath path (const QPainterPath &qpath)
{
  SkPath path;
  int count = qpath.elementCount ();
  for (int i = 0; i < count; i++)
    {
      QPainterPath::Element elem = qpath.elementAt (i);
      switch  (elem.type)
        {
        case QPainterPath::MoveToElement:
          path.moveTo (point (elem));
          break;
        case QPainterPath::LineToElement:
          path.lineTo (point (elem));
          break;
        case QPainterPath::CurveToElement:
          {
            QPainterPath::Element control2 = qpath.elementAt (++i);
            QPainterPath::Element end = qpath.elementAt (++i);
            path.cubicTo (point (elem), point (control2), point (end));
            break;
          }
        default:
          break;
        }
    }

  if (count == 1)
    {
      path.lineTo (point (qpath.elementAt (0)));
      path.close ();
    }

  if (   count > 3
         && are_equal (qpath.elementAt (count - 1).x, qpath.elementAt (0).x)
         && are_equal (qpath.elementAt (count - 1).y, qpath.elementAt (0).y)
     )
    path.close ();
  // Qt doesn't have any special definition if path is closed expect that it's first and last points are equal

  path.setFillType ((SkPath::FillType)fill_rule (qpath.fillRule ()));
  return path;
}

SkPath path (const svg_path &path)
{
  SkPath sk_path;
  for (const auto &subpath : path.subpath ())
    {
      auto elements = subpath.elements ();
      sk_path.moveTo (point (elements.front ().point));
      for (size_t i = 0; i < elements.size (); i++)
        {
          size_t next_element = i + 1;
          if (next_element == elements.size ())
            {
              if (!subpath.is_closed ())
                continue;
              else
                next_element = 0;
            }

          sk_path.cubicTo (point (elements[i].c2), point (elements[next_element].c1), point (elements[next_element].point));
        }

      if (subpath.is_closed ())
        sk_path.close ();
    }

  return sk_path;
}

SkColor color (const QColor &color)
{
  return SkColorSetARGBInline (color.alpha (), color.red (), color.green (), color.blue ());
}

SkBitmap image (const QImage &image_arg)
{
  SkBitmap bitmap = SkBitmap ();
  bitmap.setConfig (SkBitmap::Config::kARGB_8888_Config, image_arg.width (), image_arg.height ());
  bitmap.setPixels ((void *) image_arg.bits ());
  return bitmap;
}

QImage qimage (const SkBitmap &bitmap)
{
  SkAutoLockPixels image_lock (bitmap);
  return QImage ((unsigned char *)bitmap.getPixels (), bitmap.width (), bitmap.height (), (int)bitmap.rowBytes (), QImage::Format_ARGB32_Premultiplied);
}

SkRect rect (const QRect &rect)
{
  return SkRect::MakeXYWH (rect.x (), rect.y (), rect.width (), rect.height ());
}

SkRect rect (const QRectF &rect)
{
  return SkRect::MakeXYWH (rect.x (), rect.y (), rect.width (), rect.height ());
}

SkIRect Irect (const QRect &rect)
{
  return SkIRect::MakeXYWH (rect.x (), rect.y (), rect.width (), rect.height ());
}

SkIRect Irect (const QRectF &rect)
{
  return SkIRect::MakeXYWH (rect.x (), rect.y (), rect.width (), rect.height ());
}


};