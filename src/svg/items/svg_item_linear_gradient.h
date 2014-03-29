#ifndef SVG_ITEM_LINEAR_GRADIENT_H
#define SVG_ITEM_LINEAR_GRADIENT_H

#include "svg/items/svg_base_items_gradient.h"

class svg_item_linear_gradient : public svg_base_items_gradient
{
  SVG_ITEM
public:
  svg_item_linear_gradient (svg_document *document);
  virtual ~svg_item_linear_gradient () override;

protected:
  virtual renderer_base_gradient_item *create_gradient () const override;
  virtual gradient_type_t gradient_type () const override { return gradient_type_t::LINEAR; }

};

#endif // SVG_ITEM_LINEAR_GRADIENT_H
