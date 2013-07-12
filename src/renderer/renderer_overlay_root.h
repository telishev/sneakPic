#ifndef RENDERER_OVERLAY_ROOT_H
#define RENDERER_OVERLAY_ROOT_H

#include <renderer/abstract_renderer_item.h>

class renderer_overlay_root : public abstract_renderer_item
{
  QRectF m_bbox;
public:
  renderer_overlay_root (const std::string &name);
  ~renderer_overlay_root ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box () const override;
  virtual void update_bbox () override;
};

#endif // RENDERER_OVERLAY_ROOT_H
