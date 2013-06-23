#include "svg/items/svg_item_svg.h"

svg_item_svg::svg_item_svg (svg_document *document)
  : svg_named_item (document)
{

}

svg_item_svg::~svg_item_svg ()
{

}

void svg_item_svg::post_read ()
{
  return svg_named_item::post_read ();
}
