#include "svg/items/svg_item_radial_gradient.h"

#include "svg/attributes/svg_attributes_length_type.h"

#include "renderer/renderer_items_gradient.h"


svg_item_radial_gradient::svg_item_radial_gradient (svg_document *document)
  : svg_base_items_gradient (document)
{

}

svg_item_radial_gradient::~svg_item_radial_gradient ()
{

}

renderer_base_gradient_item *svg_item_radial_gradient::create_gradient () const 
{
  static const svg_attribute_cx default_cx (0.5);
  static const svg_attribute_cy default_cy (0.5);
  static const svg_attribute_r default_r (0.5);
  const svg_attribute_cx *cx = get_computed_attribute<svg_attribute_cx> (&default_cx);
  const svg_attribute_cy *cy = get_computed_attribute<svg_attribute_cy> (&default_cy);
  const svg_attribute_r   *r = get_computed_attribute<svg_attribute_r> (&default_r);
  const svg_attribute_fx *fx = get_computed_attribute<svg_attribute_fx> ();
  const svg_attribute_fy *fy = get_computed_attribute<svg_attribute_fy> ();

  double fx_val = fx->is_empty () ? cx->value () : fx->value ();
  double fy_val = fy->is_empty () ? cy->value () : fy->value ();

  return new renderer_radial_gradient (cx->value (), cy->value (), r->value (), fx_val, fy_val);
}

