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

class svg_path;

typedef unsigned int SkColor;

class qt2skia
{
public:
  static SkMatrix matrix (const QTransform &tr);
  static SkPoint point (const QPointF &point);
  static SkPath path (const QPainterPath &qpath);
  static SkPath path (const svg_path &path);
  static SkColor color (const QColor &color);
  static QImage qimage (const SkBitmap &img);
  static SkBitmap image (const QImage &image_arg);
  static SkRect rect (const QRect &rect);
  static SkRect rect (const QRectF &rect);
  static SkIRect Irect (const QRect &rect);
  static int fill_rule (int rule);
  static SkIRect Irect (const QRectF &rect);
  
};

#endif // QT2SKIA_H
