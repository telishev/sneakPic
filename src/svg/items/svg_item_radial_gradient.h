#ifndef SVG_ITEM_RADIAL_GRADIENT_H
#define SVG_ITEM_RADIAL_GRADIENT_H

#include "svg/items/svg_base_items_gradient.h"

class svg_item_radial_gradient : public svg_base_items_gradient
{
  SVG_ITEM
public:
  svg_item_radial_gradient (svg_document *document);
  virtual ~svg_item_radial_gradient () override;

protected:
  virtual renderer_base_gradient_item *create_gradient () const override;
  virtual gradient_type_t gradient_type () const { return gradient_type_t::RADIAL; }

};

#endif // SVG_ITEM_RADIAL_GRADIENT_H
