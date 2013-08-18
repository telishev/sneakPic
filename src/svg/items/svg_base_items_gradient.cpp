#include "svg/items/svg_base_items_gradient.h"

#include "renderer/renderer_items_gradient.h"

#include "svg/attributes/svg_attributes_enum.h"
#include "svg/items/svg_item_stop.h"
#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/attributes/svg_attribute_gradient_transform.h"
#include "svg/items/svg_item_type.h"


renderer_paint_server *svg_base_items_gradient::create_paint_server () const
{
  renderer_base_gradient_item *gradient = create_gradient ();
  fill_gradient (gradient);
  return gradient;
}

/// try to get own atttribute, on failure, get it from href
template<typename T>
static const T *gradient_get_attribute (const abstract_svg_item *item, const abstract_svg_item *href)
{
  const T *attribute = item->get_computed_attribute<T> ();
  if (!attribute->is_empty () || !href)
    return attribute;

  return href->get_computed_attribute<T> ();
}

bool svg_base_items_gradient::fill_gradient (renderer_base_gradient_item *gradient) const
{
  /// TODO:support gradientUnits
  const abstract_svg_item *href = get_computed_attribute<svg_attribute_xlink_href> ()->get_fragment ();
  const svg_attribute_gradient_units *gradient_units = gradient_get_attribute<svg_attribute_gradient_units> (this, href);
  const svg_attribute_spread_method *spread_method = gradient_get_attribute<svg_attribute_spread_method> (this, href);
  const svg_attribute_gradient_transform *transform = gradient_get_attribute<svg_attribute_gradient_transform> (this, href);

  gradient->set_gradient_units (gradient_units->value ());
  gradient->set_spread (spread_method->value ());
  gradient->set_transform (transform->transform ());

  if (!add_stops_to_gradient (this, gradient))
    add_stops_to_gradient (href, gradient);

  return true;
}

bool svg_base_items_gradient::add_stops_to_gradient (const abstract_svg_item *parent, renderer_base_gradient_item *gradient) const
{
  bool stops_found = false;
  for (int i = 0; i < parent->children_count (); i++)
    {
      const abstract_svg_item *child = parent->child (i);
      if (child->type () != svg_item_type::STOP)
        continue;

      const svg_item_stop *cur_stop = static_cast<const svg_item_stop *>(child);
      gradient->add_stop (cur_stop->offset (), cur_stop->color ());
      stops_found = true;
    }

  return stops_found;
}
