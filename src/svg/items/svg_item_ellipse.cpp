#include "svg/items/svg_item_ellipse.h"

#include "svg/attributes/svg_attributes_length_type.h"

#include <QPainterPath>

svg_item_ellipse::svg_item_ellipse (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_ellipse::~svg_item_ellipse ()
{
}


QPainterPath svg_item_ellipse::get_path () const 
{
  const svg_attribute_cx *cx = get_computed_attribute<svg_attribute_cx> ();
  const svg_attribute_cy *cy = get_computed_attribute<svg_attribute_cy> ();
  const svg_attribute_rx *rx = get_computed_attribute<svg_attribute_rx> ();
  const svg_attribute_ry *ry = get_computed_attribute<svg_attribute_ry> ();

  QPainterPath path;
  path.addEllipse (QPointF (cx->value (), cy->value ()), rx->value (), ry->value ());
  return path;
}


