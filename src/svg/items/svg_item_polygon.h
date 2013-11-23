#ifndef SVG_ITEM_POLYGON_H
#define SVG_ITEM_POLYGON_H

#include "svg/items/svg_base_shape_item.h"


class svg_item_polygon : public svg_base_shape_item
{
  SVG_ITEM

public:
  svg_item_polygon (svg_document *document);
  virtual ~svg_item_polygon () override;

protected:
  virtual QPainterPath get_path () const override;
};


#endif // SVG_ITEM_POLYGON_H
