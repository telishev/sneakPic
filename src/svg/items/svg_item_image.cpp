#include "svg/items/svg_item_image.h"

#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attribute_xlink_href.h"

#include "renderer/renderer_item_image.h"
#include "renderer/renderer_overlay_path.h"

#include <QImage>
#include <QPainterPath>

svg_item_image::svg_item_image (svg_document *document)
  : abstract_svg_item (document)
{
}

svg_item_image::~svg_item_image ()
{
}

bool svg_item_image::check_item ()
{
  return true;
}

abstract_renderer_item *svg_item_image::create_renderer_item () const
{
  const svg_attribute_xlink_href *xlink_href = get_computed_attribute<svg_attribute_xlink_href> ();
  if (!xlink_href->has_image_data ())
    return 0;

  renderer_item_image *render_item = new renderer_item_image (name ().toStdString ());
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  const svg_attribute_width *width = get_computed_attribute<svg_attribute_width> ();
  const svg_attribute_height *height = get_computed_attribute<svg_attribute_height> ();
  const svg_attribute_display *display = get_computed_attribute<svg_attribute_display> ();

  render_item->set_dimensions (x->value (), y->value (), width->value (), height->value ());
  render_item->set_image_data (*xlink_href->get_image_data ());
  render_item->set_display (display->value ());
  return render_item;
}

abstract_renderer_item *svg_item_image::create_overlay_item () const 
{
  QPainterPath path;
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  const svg_attribute_width *width = get_computed_attribute<svg_attribute_width> ();
  const svg_attribute_height *height = get_computed_attribute<svg_attribute_height> ();
  path.addRect (x->value (), y->value (), width->value (), height->value ());
  renderer_overlay_path *overlay_item = new renderer_overlay_path (name ().toStdString ());
  overlay_item->set_painter_path (path);
  return overlay_item;
}
