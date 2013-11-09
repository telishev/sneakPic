#include "svg/items/svg_item_circle.h"

#include "svg/attributes/svg_attributes_length_type.h"

#include <QPainterPath>

svg_item_circle::svg_item_circle (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_circle::~svg_item_circle ()
{
}

bool svg_item_circle::check_item ()
{
  return true;
}

QPainterPath svg_item_circle::get_path () const
{
  const svg_attribute_cx *cx = get_computed_attribute<svg_attribute_cx> ();
  const svg_attribute_cy *cy = get_computed_attribute<svg_attribute_cy> ();
  const svg_attribute_r *r = get_computed_attribute<svg_attribute_r> ();

  QPainterPath path;
  path.addEllipse (QPointF (cx->value (), cy->value ()), r->value (), r->value ());
  return path;
}



