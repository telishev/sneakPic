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
  QPainterPath m_clip_path;
  bool m_has_clip_path;
  bool visible;
  display m_display;

public:
  renderer_graphics_item (const std::string &name);
  virtual ~renderer_graphics_item ();

  void set_clip_path (const QPainterPath &path) { m_clip_path = path; m_has_clip_path = true; }
  void set_visibility (bool visible_arg);
  void set_display (display display) { m_display = display; }
  void set_opacity (double opacity_arg);

  virtual void draw_graphics_item (SkCanvas &canvas, const renderer_config *config) const = 0;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }
  virtual void update_bbox () override = 0;

  void set_bounding_box (const QRectF &rect) { m_bbox = rect; }
};

#endif // RENDERER_GRAPHICS_ITEM_H
