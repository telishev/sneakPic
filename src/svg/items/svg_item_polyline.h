#ifndef SVG_ITEM_POLYLINE_H
#define SVG_ITEM_POLYLINE_H

#include "svg/items/svg_base_shape_item.h"


class svg_item_polyline : public svg_base_shape_item
{
  SVG_ITEM

public:
  svg_item_polyline (svg_document *document);
  virtual ~svg_item_polyline () override;

protected:
  virtual QPainterPath get_path () const override;
};


#endif // SVG_ITEM_POLYLINE_H
