#include "svg/items/svg_item_group.h"

svg_item_group::svg_item_group (svg_document *document)
  : abstract_svg_item (document)
{

}

svg_item_group::~svg_item_group ()
{

}

void svg_item_group::draw (QPainter &painter)
{
  for (abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    child->draw (painter);
}



