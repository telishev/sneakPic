#include "svg/items/svg_base_shape_item.h"

#include "renderer/renderer_base_shape_item.h"

#include "svg/attributes/svg_attribute_style.h"
#include "svg/attributes/svg_attribute_fill.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attribute_stroke.h"
#include "svg/attributes/svg_attribute_transform.h"


void svg_base_shape_item::update_base_item (renderer_base_shape_item *item)
{
  const svg_attribute_fill *fill = get_computed_attribute<svg_attribute_fill> ();
  const svg_attribute_stroke *stroke = get_computed_attribute<svg_attribute_stroke> ();
  const svg_attribute_stroke_width *stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();
  const svg_attribute_transform *transform = get_computed_attribute<svg_attribute_transform> ();

  item->set_fill_color (fill->get_color ());
  item->set_show_fill (fill->is_applied ());
  item->set_stroke_width (stroke_width->get_stroke_width ());
  item->set_show_stroke (stroke->is_applied ());
  item->set_stroke_color (stroke->get_color ());
  item->set_transform (transform->computed_transform ());
}
