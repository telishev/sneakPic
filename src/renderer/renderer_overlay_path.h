#ifndef RENDERER_OVERLAY_PATH_H
#define RENDERER_OVERLAY_PATH_H

#include "renderer/renderable_item.h"

#include <QPainterPath>
#include <QRectF>
#include <QColor>

class renderer_overlay_path : public renderable_item
{
  QPainterPath m_path;
  QRectF m_bbox;
  QColor m_color;
  int m_xfer_mode; // this int here is actually SkXfermode::Mode
  bool m_visible;

public:
  renderer_overlay_path ();
  ~renderer_overlay_path ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

  void set_painter_path (const QPainterPath &path);
  void set_color (const QColor &color);
  void set_xfer_mode (int mode); // this int here is actually SkXfermode::Mode
  void set_visible (bool visible) { m_visible = visible; }
};

#endif // RENDERER_OVERLAY_PATH_H
