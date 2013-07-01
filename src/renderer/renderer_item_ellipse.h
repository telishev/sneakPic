#ifndef RENDERER_ITEM_ELLIPSE_H
#define RENDERER_ITEM_ELLIPSE_H

#include "renderer/renderer_base_shape_item.h"

class renderer_item_ellipse : public renderer_base_shape_item
{
  QRectF m_bbox;
  double m_cx, m_cy, m_rx, m_ry;

public:
  renderer_item_ellipse ();
  ~renderer_item_ellipse ();

  virtual void draw (QPainter &painter) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }

  void set_ellipse (double cx, double cy, double rx, double ry); 
};


#endif // RENDERER_ITEM_CIRCLE_H
