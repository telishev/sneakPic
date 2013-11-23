#ifndef SVG_ITEM_RECT_H
#define SVG_ITEM_RECT_H

#include "svg/items/svg_base_shape_item.h"

class svg_item_rect : public svg_base_shape_item
{
  SVG_ITEM

public:
  svg_item_rect (svg_document *document);
  virtual ~svg_item_rect () override;

protected:
  virtual QPainterPath get_path () const override;

};

#endif // SVG_ITEM_RECT_H
