#ifndef SVG_ITEM_LINE_H
#define SVG_ITEM_LINE_H

#include "svg/items/svg_base_shape_item.h"

class svg_item_line : public svg_base_shape_item
{
  SVG_ITEM

public:
  svg_item_line (svg_document *document);
  virtual ~svg_item_line () override;

protected:
  virtual QPainterPath get_path () const override;

};


#endif // SVG_ITEM_LINE_H
