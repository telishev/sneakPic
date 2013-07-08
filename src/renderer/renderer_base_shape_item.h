#ifndef RENDERER_BASE_SHAPE_ITEM_H
#define RENDERER_BASE_SHAPE_ITEM_H

#include "renderer/abstract_renderer_item.h"

#include <QPen>
#include <QBrush>
#include <QPainterPath>

class renderer_paint_server;
class SkPaint;

class renderer_base_shape_item : public abstract_renderer_item
{
protected:
  QPainterPath m_clip_path;
  bool m_has_clip_path;
  bool visible;
  SkPaint *m_stroke;
  SkPaint *m_fill;

public:
  renderer_base_shape_item ();
  ~renderer_base_shape_item ();

  void set_stroke_linecap (Qt::PenCapStyle linecap);
  void set_stroke_linejoin (Qt::PenJoinStyle linejoin);
  void set_stroke_miterlimit (double miterlimit);
  void set_stroke_width (double width);
  void set_visibility (bool visible_arg);

  void set_stroke_server (const renderer_paint_server *server);
  void set_fill_server (const renderer_paint_server *server);
  void set_clip_path (const QPainterPath &path) { m_clip_path = path; m_has_clip_path = true; }

protected:
  bool configure_painter (SkPaint &paint, bool stroke) const;
  void adjust_bbox (QRectF &bbox) const;
};

#endif // RENDERER_BASE_SHAPE_ITEM_H
