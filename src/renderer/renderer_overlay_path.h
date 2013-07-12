#ifndef RENDERER_OVERLAY_PATH_H
#define RENDERER_OVERLAY_PATH_H

#include "renderer/abstract_renderer_item.h"

class renderer_overlay_path : public abstract_renderer_item
{
  QPainterPath m_path;
  QRectF m_bbox;

public:
  renderer_overlay_path (const std::string &name);
  ~renderer_overlay_path ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }
  virtual void update_bbox () override;

  void set_painter_path (const QPainterPath &path);
};

#endif // RENDERER_OVERLAY_PATH_H
