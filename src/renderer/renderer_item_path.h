#ifndef RENDERER_ITEM_PATH_H
#define RENDERER_ITEM_PATH_H

#include "renderer/abstract_renderer_item.h"

#include <QPainterPath>
#include <QPen>
#include <QBrush>

class renderer_item_path : public abstract_renderer_item
{
  QPainterPath m_path;
  QPen m_pen;
  QBrush m_brush;

public:
  renderer_item_path ();
  ~renderer_item_path ();

  virtual void draw (QPainter &painter) const override;

  void set_stroke_width (double width);
  void set_stroke_color (const QColor &color);
  void set_show_stroke (bool show);

  void set_fill_color (const QColor &color);
  void set_show_fill (bool show);
  void set_painter_path (const QPainterPath &path);
};


#endif // RENDERER_ITEM_PATH_H
