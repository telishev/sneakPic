#ifndef SVG_ITEM_ELLIPSE_H
#define SVG_ITEM_ELLIPSE_H

#include "svg/items/svg_base_shape_item.h"


class svg_item_ellipse : public svg_base_shape_item
{
  SVG_ITEM

public:
  svg_item_ellipse (svg_document *document);
  virtual ~svg_item_ellipse () override;

protected:
  virtual bool check_item () override;
  virtual QPainterPath get_path () const override;

};


#endif // SVG_ITEM_ELLIPSE_H
