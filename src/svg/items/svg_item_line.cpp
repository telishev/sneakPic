#include "svg/items/svg_item_line.h"

#include "svg/attributes/svg_attributes_length_type.h"

#include <QPainterPath>

svg_item_line::svg_item_line (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_line::~svg_item_line ()
{
}

QPainterPath svg_item_line::get_path () const 
{
  const svg_attribute_x1 *x1 = get_computed_attribute<svg_attribute_x1> ();
  const svg_attribute_x2 *x2 = get_computed_attribute<svg_attribute_x2> ();
  const svg_attribute_y1 *y1 = get_computed_attribute<svg_attribute_y1> ();
  const svg_attribute_y2 *y2 = get_computed_attribute<svg_attribute_y2> ();

  QPainterPath path;
  path.moveTo (x1->value (), y1->value ());
  path.lineTo (x2->value (), y2->value ());
  return path;
}

