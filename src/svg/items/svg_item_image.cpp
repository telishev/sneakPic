#include "svg/items/svg_item_image.h"

#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attribute_xlink_href.h"

#include "svg/svg_document.h"

#include "renderer/renderer_item_image.h"
#include "renderer/renderer_overlay_path.h"
#include "renderer/overlay_item_type.h"

#include <QImage>
#include <QPainterPath>
#include "renderer/renderer_item_selection.h"



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

abstract_renderer_item *svg_item_image::create_overlay_item (overlay_item_type overlay_type) const 
{
  QPainterPath path;
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  const svg_attribute_width *width = get_computed_attribute<svg_attribute_width> ();
  const svg_attribute_height *height = get_computed_attribute<svg_attribute_height> ();
  QRectF rect (x->value (), y->value (), width->value (), height->value ());

  switch (overlay_type)
    {
    case overlay_item_type::CURRENT_ITEM:
      {
        path.addRect (rect);
        renderer_overlay_path *overlay_item = new renderer_overlay_path (document ()->create_overlay_name ().toStdString ());
        overlay_item->set_painter_path (path);
        return overlay_item;
      }
    case overlay_item_type::SELECTION:
      {
        renderer_item_selection *selection = new renderer_item_selection (document ()->create_overlay_name ().toStdString ());
        selection->set_bbox (rect);
        return selection;
      }
    }
  return nullptr;
}
