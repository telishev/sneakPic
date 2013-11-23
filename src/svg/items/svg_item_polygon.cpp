#include "svg/items/svg_item_polygon.h"

#include "svg/attributes/svg_attribute_points.h"

#include <QPainterPath>

svg_item_polygon::svg_item_polygon (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_polygon::~svg_item_polygon ()
{
}


QPainterPath svg_item_polygon::get_path () const 
{
  QPainterPath path;
  const svg_attribute_points *points = get_computed_attribute <svg_attribute_points> ();
  if (points->value ().size () == 0)
    return path;

  path.moveTo (points->value ()[0]);
  for (int i = 0; i < points->value ().size (); i++)
    path.lineTo (points->value ()[i]);
  path.closeSubpath ();
  return path;
}
