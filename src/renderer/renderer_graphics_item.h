#ifndef RENDERER_GRAPHICS_ITEM_H
#define RENDERER_GRAPHICS_ITEM_H

#include "renderer/abstract_renderer_item.h"

#include <QPainterPath>

class renderer_paint_server;
class SkPaint;
enum class display;

class renderer_graphics_item : public abstract_renderer_item
{
protected:
  QRectF m_bbox;
  QRectF m_bbox_computed;
  QPainterPath m_clip_path;
  bool m_has_clip_path;
  bool visible;
  bool m_ignore_bbox;
  display m_display;
  double m_opacity;

public:
  renderer_graphics_item (const string &name);
  virtual ~renderer_graphics_item ();

  void set_clip_path (const QPainterPath &path) { m_clip_path = path; m_has_clip_path = true; }
  void set_visibility (bool visible);
  void set_display (display display) { m_display = display; }
  void set_opacity (double opacity_arg);
  void set_ignore_bbox (bool ignore) { m_ignore_bbox = ignore; }

  virtual void draw_graphics_item (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const = 0;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box_impl () const override { return m_bbox_computed; }
  virtual void update_bbox_impl () override { m_bbox_computed = m_bbox; };

  void set_bounding_box (const QRectF &rect) { m_bbox = rect; }
};

#endif // RENDERER_GRAPHICS_ITEM_H
