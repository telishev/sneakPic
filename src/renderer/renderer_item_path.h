#ifndef RENDERER_ITEM_PATH_H
#define RENDERER_ITEM_PATH_H

#include "renderer/renderer_base_shape_item.h"

#include <QPainterPath>

class QTransform;

class renderer_item_path : public renderer_base_shape_item
{
  QPainterPath m_path;
  QRectF m_bbox;

public:
  renderer_item_path (const std::string &name);
  ~renderer_item_path ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }
  virtual void update_bbox () override;

  void set_painter_path (const QPainterPath &path);
};


#endif // RENDERER_ITEM_PATH_H
