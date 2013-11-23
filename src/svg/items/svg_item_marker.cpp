#include "svg/items/svg_item_marker.h"

#include "svg/items/svg_graphics_item.h"

#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attribute_orient.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attribute_viewbox.h"

#include "renderer/renderer_graphics_item.h"

#include <QPainterPath>

svg_item_marker::svg_item_marker (svg_document *document)
  : abstract_svg_item (document)
{
}

svg_item_marker::~svg_item_marker ()
{
}

abstract_renderer_item *svg_item_marker::create_renderer_item_for_marker (QPointF target_point, QPointF bisector, QTransform transform, double stroke_width)
{
  auto attr_marker_units = get_computed_attribute <svg_attribute_marker_units> ();
  auto refx = get_computed_attribute <svg_attribute_refx> ();
  auto refy = get_computed_attribute <svg_attribute_refy> ();
  auto marker_width = get_computed_attribute <svg_attribute_marker_width> ();
  auto marker_height = get_computed_attribute <svg_attribute_marker_height> ();
  auto orient = get_computed_attribute <svg_attribute_orient> ();
  auto view_box = get_computed_attribute <svg_attribute_view_box> ();
  double multiplier;

  svg_graphics_item *child_item = child (0)->to_graphics_item ();
  if (!child_item)
    return nullptr;

  child_item->update_bbox ();
  renderer_graphics_item *renderer_item = static_cast<renderer_graphics_item *> (child_item->create_renderer_item ());
  renderer_item->update_bbox_impl ();
  QRectF bbox = renderer_item->bounding_box_impl ();


  QTransform add_transform;
  if (attr_marker_units->value () == marker_units::STROKE_WIDTH)
    multiplier = stroke_width;
  else
    multiplier = 1.0;

  QTransform viewport_transform;
  if (view_box && view_box->get_width () > 0.0 && view_box->get_height () > 0.0)
    viewport_transform = view_box->get_transform (QRectF (0.0, 0.0, marker_width->value () * multiplier, marker_height->value () * multiplier));
  add_transform.translate (target_point.x () - refx->value () * multiplier, (target_point.y () - refy->value () * multiplier));
  add_transform.scale (multiplier, multiplier);
  add_transform.rotate (-orient->value (bisector));
  FIX_UNUSED (transform);
  renderer_item->set_transform (renderer_item->transform () * add_transform);
  renderer_item->set_bounding_box (add_transform.mapRect (bbox));
  return renderer_item;
}

