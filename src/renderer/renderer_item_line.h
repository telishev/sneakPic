#ifndef RENDERER_ITEM_LINE_H
#define RENDERER_ITEM_LINE_H

#include "renderer/renderer_base_shape_item.h"

class renderer_item_line : public renderer_base_shape_item
{
  QLineF m_line;
  QRectF m_bbox;

public:
  renderer_item_line ();
  ~renderer_item_line ();

  virtual void draw (QPainter &painter) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }

  void set_line (double x1, double y1, double x2, double y2);
};


#endif // RENDERER_ITEM_LINE_H
