#ifndef QT2SKIA_H
#define QT2SKIA_H
#include <Qt>

#pragma warning(push, 0)
#include <SkPath.h>
#pragma warning(pop)

class SkMatrix;
struct SkPoint;
class SkPaint;
class SkBitmap;
struct SkRect;

class QPointF;
class QTransform;
class QPainterPath;
class QColor;
class QBrush;
class QImage;
class QRect;
class QRectF;

typedef unsigned int SkColor;

class qt2skia
{
public:
  static SkMatrix matrix (const QTransform &tr);
  static SkPoint point (const QPointF &point);
  static SkPath path (const QPainterPath &qpath);
  static SkColor color (const QColor &color);
  static QImage qimage (const SkBitmap &img);
  static SkBitmap image (const QImage &image_arg);
  static SkRect rect (const QRect &rect);
  static SkRect rect (const QRectF &rect);
  static SkIRect Irect (const QRect &rect);
  static SkPath::FillType fill_rule (Qt::FillRule rule);
  static SkIRect Irect (const QRectF &rect);
};

#endif // QT2SKIA_H
