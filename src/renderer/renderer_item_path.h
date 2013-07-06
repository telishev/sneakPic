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
  renderer_item_path ();
  ~renderer_item_path ();

  virtual void draw (QPainter &painter, const renderer_state &state) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }

  void set_painter_path (const QPainterPath &path);
};


#endif // RENDERER_ITEM_PATH_H
