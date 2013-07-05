#ifndef SVG_BASE_ITEMS_GRADIENT_H
#define SVG_BASE_ITEMS_GRADIENT_H

#include "svg/items/abstract_svg_item.h"

class renderer_base_gradient_item;
class renderer_paint_server;

class svg_base_items_gradient : public abstract_svg_item
{

public:
  svg_base_items_gradient (svg_document *document) : abstract_svg_item (document) {}
  virtual ~svg_base_items_gradient () override {}

  renderer_paint_server *create_paint_server () const;
  bool fill_gradient (renderer_base_gradient_item *gradient) const;

  virtual const abstract_renderer_item *get_renderer_item () const override { return nullptr; }

protected:
  virtual renderer_base_gradient_item *create_gradient () const = 0;
};

#endif // SVG_BASE_ITEMS_GRADIENT_H
