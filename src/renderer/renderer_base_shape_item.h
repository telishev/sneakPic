#ifndef RENDERER_BASE_SHAPE_ITEM_H
#define RENDERER_BASE_SHAPE_ITEM_H

#include "renderer/renderer_graphics_item.h"

#include <QPainterPath>

class renderer_paint_server;
class SkPaint;

class renderer_base_shape_item : public renderer_graphics_item
{
protected:
  QPainterPath m_path;
  SkPaint *m_stroke;
  SkPaint *m_fill;
  renderer_paint_server *m_stroke_server;
  renderer_paint_server *m_fill_server;
  vector <abstract_renderer_item *> marker_renderer_items;

public:
  renderer_base_shape_item (const string &name);
  ~renderer_base_shape_item ();

  void set_stroke_linecap (Qt::PenCapStyle linecap);
  void set_stroke_linejoin (Qt::PenJoinStyle linejoin);
  void set_stroke_miterlimit (double miterlimit);
  void set_stroke_width (double width);
  void set_dash_array (QList<double> dash_array, double offset);

  void set_stroke_server (const renderer_paint_server *server);
  void set_fill_server (const renderer_paint_server *server);

  virtual void draw_graphics_item (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual void update_bbox_impl () override;

  void set_painter_path (const QPainterPath &path) { m_path = path; }
  void set_bounding_box (const QRectF &rect) { m_bbox = rect; }

  const QPainterPath & painter_path () { return m_path; }

  void add_marker (abstract_renderer_item *marker);

protected:
  bool configure_painter (SkPaint &paint, bool stroke, bool config_for_selection) const;
};

#endif // RENDERER_BASE_SHAPE_ITEM_H
