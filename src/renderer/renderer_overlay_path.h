#ifndef RENDERER_OVERLAY_PATH_H
#define RENDERER_OVERLAY_PATH_H

#include "renderer/renderable_item.h"

#include <QPainterPath>
#include <QRectF>

class renderer_overlay_path : public renderable_item
{
  QPainterPath m_path;
  QRectF m_bbox;

public:
  renderer_overlay_path ();
  ~renderer_overlay_path ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

  void set_painter_path (const QPainterPath &path);
};

#endif // RENDERER_OVERLAY_PATH_H
