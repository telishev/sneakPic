#include "svg/items/svg_item_svg.h"

#include "common/memory_deallocation.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attribute_viewbox.h"


#include "renderer/renderer_item_svg.h"



svg_item_svg::svg_item_svg (svg_document *document)
  : svg_item_group_type (document)
{
}

svg_item_svg::~svg_item_svg ()
{
}

double svg_item_svg::width () const
{
  static const svg_attribute_width default_width (210, svg_length_units::MM); /// A4
  return get_computed_attribute<svg_attribute_width> (&default_width)->value ();
}

double svg_item_svg::height () const
{
  static const svg_attribute_height default_height (297, svg_length_units::MM); /// A4
  return get_computed_attribute<svg_attribute_height> (&default_height)->value ();
}

void svg_item_svg::update_renderer_item ()
{

}

abstract_renderer_item *svg_item_svg::create_renderer_item_impl () const
{
  renderer_item_svg *render_item = new renderer_item_svg (name ());

  update_group_item (render_item);
  return render_item;
}

bool svg_item_svg::check_item ()
{
  const svg_attribute_view_box *view_box = get_computed_attribute<svg_attribute_view_box> ();
  if (!view_box->is_empty ())
    {
      QRectF viewport = QRectF (0, 0, width (), height ());
      auto transform = get_attribute_for_change<svg_attribute_transform> ();
      transform->set_additional_transform (view_box->get_transform (viewport));
    }

  return true;
}
