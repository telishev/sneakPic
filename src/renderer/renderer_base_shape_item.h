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

public:
  renderer_base_shape_item ();
  ~renderer_base_shape_item ();

  void set_stroke_width (double width);
  void set_stroke_color (const QColor &color);
  void set_show_stroke (bool show);

  void set_fill_color (const QColor &color);
  void set_show_fill (bool show);

protected:
  void configure_painter (QPainter &painter) const;
  
};

#endif // RENDERER_BASE_SHAPE_ITEM_H
