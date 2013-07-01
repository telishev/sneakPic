#ifndef SVG_BASE_SHAPE_ITEM_H
#define SVG_BASE_SHAPE_ITEM_H

#include "svg/items/abstract_svg_item.h"

class renderer_base_shape_item;

class svg_base_shape_item : public abstract_svg_item
{

public:
  svg_base_shape_item (svg_document *document) : abstract_svg_item (document) {}
  virtual ~svg_base_shape_item () override {}

  virtual bool render_children () const { return false; }

protected:
  void update_base_item (renderer_base_shape_item *item);

};

#endif // SVG_BASE_SHAPE_ITEM_H
