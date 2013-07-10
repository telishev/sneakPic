#ifndef RENDERER_ITEM_GROUP_H
#define RENDERER_ITEM_GROUP_H

#include <renderer/abstract_renderer_item.h>
#include <QPainterPath>
#include <vector>
#include <string>

class abstract_svg_item;

class renderer_item_group : public abstract_renderer_item
{
  double m_opacity;
  QRectF m_bbox;
  QPainterPath m_clip_path;
  bool m_has_clip_path;

public:
  renderer_item_group (const std::string &name);
  ~renderer_item_group ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state) const override;
  virtual QRectF bounding_box () const override;
  virtual void update_bbox () override;
  void set_opacity (double opacity) { m_opacity = opacity; }
  void set_clip_path (const QPainterPath &path) { m_clip_path = path; m_has_clip_path = true; }  
};

#endif // RENDERER_ITEM_GROUP_H
