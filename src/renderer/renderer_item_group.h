#ifndef RENDERER_ITEM_GROUP_H
#define RENDERER_ITEM_GROUP_H

#include <renderer/abstract_renderer_item.h>
#include <QPainterPath>
#include <vector>
#include <string>

enum class display;
class abstract_svg_item;

class renderer_item_group : public abstract_renderer_item
{
  double m_opacity;
  display m_display;
  QRectF m_bbox;
  QPainterPath m_clip_path;
  bool m_has_clip_path;

public:
  renderer_item_group (const string &name);
  ~renderer_item_group ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box_impl () const override;
  virtual void update_bbox_impl () override;
  void set_opacity (double opacity) { m_opacity = opacity; }
  void set_display (display display_arg) { m_display = display_arg; }
  void set_clip_path (const QPainterPath &path) { m_clip_path = path; m_has_clip_path = true; }  
};

#endif // RENDERER_ITEM_GROUP_H
