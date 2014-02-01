#ifndef QT2SKIA_H
#define QT2SKIA_H

class SkMatrix;
struct SkPoint;
class SkPaint;
class SkBitmap;
struct SkRect;
struct SkIRect;
class SkPath;

class QPointF;
class QTransform;
class QPainterPath;
class QColor;
class QBrush;
class QImage;
class QRect;
class QRectF;

class svg_path_geom;

typedef unsigned int SkColor;

namespace qt2skia
{
  SkMatrix matrix (const QTransform &tr);
  SkPoint point (const QPointF &point);
  SkPath path (const QPainterPath &qpath);
  SkPath path (const svg_path_geom &path);
  SkColor color (const QColor &color);
  QImage qimage (const SkBitmap &img);
  SkBitmap image (const QImage &image_arg);
  SkRect rect (const QRect &rect);
  SkRect rect (const QRectF &rect);
  SkIRect Irect (const QRect &rect);
  int fill_rule (int rule);
  SkIRect Irect (const QRectF &rect);
  
};

#endif // QT2SKIA_H
