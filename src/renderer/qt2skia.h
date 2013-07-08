#ifndef QT2SKIA_H
#define QT2SKIA_H

class SkMatrix;
class SkPoint;
class SkPath;
class SkPaint;
class SkBitmap;

class QPointF;
class QTransform;
class QPainterPath;
class QColor;
class QBrush;
class QImage;

typedef unsigned int SkColor;

class qt2skia
{
public:
  static SkMatrix matrix (const QTransform &tr);
  static SkPoint point (const QPointF &point);
  static SkPath path (const QPainterPath &qpath);
  static SkColor color (const QColor &color);
  static QImage qimage (const SkBitmap &img);
};

#endif // QT2SKIA_H
