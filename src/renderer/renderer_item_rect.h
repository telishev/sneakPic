#ifndef RENDERER_ITEM_RECT_H
#define RENDERER_ITEM_RECT_H

#include "renderer/renderer_base_shape_item.h"

class renderer_item_rect : public renderer_base_shape_item
{
  QRectF m_rect;
  QRectF m_bbox;

  double m_rx, m_ry;

public:
  renderer_item_rect ();
  ~renderer_item_rect ();

  virtual void draw (QPainter &painter) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }

  void set_rect (double x, double y, double width, double height, double rx, double ry); 
};


#endif // RENDERER_ITEM_RECT_H
