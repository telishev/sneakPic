#include "svg/items/svg_item_linear_gradient.h"

#include "svg/attributes/svg_attributes_length_type.h"

#include "renderer/renderer_items_gradient.h"


svg_item_linear_gradient::svg_item_linear_gradient (svg_document *document)
  : svg_base_items_gradient (document)
{

}

svg_item_linear_gradient::~svg_item_linear_gradient ()
{

}

bool svg_item_linear_gradient::check_item ()
{
  return true;
}

renderer_base_gradient_item *svg_item_linear_gradient::create_gradient () const 
{
  static const svg_attribute_x2 default_x2 (1.0);
  const svg_attribute_x1 *x1 = get_computed_attribute<svg_attribute_x1> ();
  const svg_attribute_y1 *y1 = get_computed_attribute<svg_attribute_y1> ();
  const svg_attribute_x2 *x2 = get_computed_attribute<svg_attribute_x2> (&default_x2);
  const svg_attribute_y2 *y2 = get_computed_attribute<svg_attribute_y2> ();

  return new renderer_linear_gradient (x1->value (), y1->value (), x2->value (), y2->value ());
}
