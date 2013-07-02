#ifndef RENDERER_BASE_SHAPE_ITEM_H
#define RENDERER_BASE_SHAPE_ITEM_H

#include "renderer/abstract_renderer_item.h"

#include <QPen>
#include <QBrush>

class renderer_base_shape_item : public abstract_renderer_item
{
protected:
  QPen m_pen;
  QBrush m_brush;
  double m_opacity;

public:
  renderer_base_shape_item ();
  ~renderer_base_shape_item ();

  void set_stroke_linecap (Qt::PenCapStyle linecap);
  void set_stroke_linejoin (Qt::PenJoinStyle linejoin);
  void set_stroke_miterlimit (double miterlimit);
  void set_stroke_width (double width);
  void set_stroke_color (const QColor &color);
  void set_show_stroke (bool show);

  void set_fill_color (const QColor &color);
  void set_show_fill (bool show);

  void set_opacity (double opacity);
  void set_fill_opacity (double opacity);
  void set_stroke_opacity (double opacity);

protected:
  void configure_painter (QPainter &painter) const;
  void adjust_bbox (QRectF &bbox) const;
};

#endif // RENDERER_BASE_SHAPE_ITEM_H
