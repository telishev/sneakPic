#include "svg/items/svg_item_image.h"

#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/attributes/svg_attribute_preserve_aspect_ratio.h"

#include "renderer/renderer_item_image.h"
#include "renderer/renderer_overlay_path.h"
#include "renderer/overlay_item_type.h"

#include <QImage>
#include <QPainterPath>
#include "renderer/renderer_item_selection.h"



svg_item_image::svg_item_image (svg_document *document)
  : svg_base_graphics_item (document)
{
}

svg_item_image::~svg_item_image ()
{
}

bool svg_item_image::check_item ()
{
  return true;
}

renderer_graphics_item *svg_item_image::create_renderer_graphics_item () const
{
  const svg_attribute_xlink_href *xlink_href = get_computed_attribute<svg_attribute_xlink_href> ();
  if (!xlink_href->has_image_data ())
    return 0;

  renderer_item_image *render_item = new renderer_item_image (name ());
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  const svg_attribute_width *width = get_computed_attribute<svg_attribute_width> ();
  const svg_attribute_height *height = get_computed_attribute<svg_attribute_height> ();
  const auto *preserve_aspect_ratio = get_computed_attribute<svg_attribute_preserve_aspect_ratio> ();
  QImage image = *xlink_href->get_image_data ();
  QRectF viewport_rect (x->value (), y->value (), width->value (), height->value ());
  QRectF src_rect (0.0, 0.0, image.width (), image.height ());
  QRectF dest_rect = viewport_rect;
  if (preserve_aspect_ratio)
    dest_rect = preserve_aspect_ratio->get_desired_rect (src_rect, viewport_rect);

  render_item->set_src_rect (src_rect);
  render_item->set_dest_rect (dest_rect);
  render_item->set_image_data (image);
  return render_item;
}

QPainterPath svg_item_image::get_boundaries () const
{
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  const svg_attribute_width *width = get_computed_attribute<svg_attribute_width> ();
  const svg_attribute_height *height = get_computed_attribute<svg_attribute_height> ();
  QPainterPath path;
  QRectF rect (x->value (), y->value (), width->value (), height->value ());
  path.addRect (rect);
  return path;
}
