#include "svg/items/svg_item_polyline.h"

#include "svg/attributes/svg_attribute_points.h"

#include <QPainterPath>

svg_item_polyline::svg_item_polyline (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_polyline::~svg_item_polyline ()
{
}

bool svg_item_polyline::check_item ()
{
  return true;
}

QPainterPath svg_item_polyline::get_path () const 
{
  QPainterPath path;
  const svg_attribute_points *points = get_computed_attribute <svg_attribute_points> ();
  path.moveTo (points->value ()[0]);
  for (int i = 0; i < points->value ().size (); i++)
    path.lineTo (points->value ()[i]);
  return path;
}
