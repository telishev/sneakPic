#include "svg/items/svg_base_items_gradient.h"

#include "renderer/renderer_items_gradient.h"

#include "svg/attributes/svg_attributes_enum.h"
#include "svg/items/svg_item_stop.h"
#include "svg/attributes/svg_attribute_xlink_href.h"


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
  const abstract_svg_item *href = get_computed_attribute<svg_attribute_xlink_href> ()->href ();
  const svg_attribute_gradient_units *gradient_units = gradient_get_attribute<svg_attribute_gradient_units> (this, href);
  const svg_attribute_spread_method *spread_method = gradient_get_attribute<svg_attribute_spread_method> (this, href);

  gradient->set_gradient_units (gradient_units->value ());
  gradient->set_spread (spread_method->value ());

  auto stops = get_childs_by_name (svg_item_stop::static_name ());
  if (stops.first == stops.second && href)
    stops = href->get_childs_by_name (svg_item_stop::static_name ());
  for (auto it = stops.first; it != stops.second; ++it)
    {
      const svg_item_stop *cur_stop = static_cast<const svg_item_stop *>(it->second);
      gradient->add_stop (cur_stop->offset (), cur_stop->color ());
    }

  return true;
}
