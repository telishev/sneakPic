#include "svg/items/svg_item_svg.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_width.h"
#include "svg/attributes/svg_attribute_height.h"

svg_item_svg::svg_item_svg (svg_document *document)
  : abstract_svg_item (document)
{
}

svg_item_svg::~svg_item_svg ()
{

}

double svg_item_svg::width () const
{
  return get_computed_attribute<svg_attribute_width> ()->width ();
}

double svg_item_svg::height () const
{
  return get_computed_attribute<svg_attribute_height> ()->height ();
}

void svg_item_svg::draw (QPainter &painter)
{
  for (abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    child->draw (painter);
}

