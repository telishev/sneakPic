#ifndef SVG_BASE_SHAPE_ITEM_H
#define SVG_BASE_SHAPE_ITEM_H

#include "svg/items/abstract_svg_item.h"

class renderer_base_shape_item;
class QPainterPath;

class svg_base_shape_item : public abstract_svg_item
{

public:
  svg_base_shape_item (svg_document *document) : abstract_svg_item (document) {}
  virtual ~svg_base_shape_item () override {}

  virtual bool render_children () const override { return false; }
  QPainterPath to_path () const;

protected:
  void update_base_item (renderer_base_shape_item *item);
  virtual QPainterPath get_path () const = 0;
};

#endif // SVG_BASE_SHAPE_ITEM_H
