#include "svg/items/svg_item_use.h"

#include "svg/attributes/svg_attribute_xlink_href.h"

svg_item_use::svg_item_use (svg_document *document)
  : abstract_svg_item (document)
{

}

svg_item_use::~svg_item_use ()
{

}

bool svg_item_use::check_item ()
{
  return update_children_tree ();
}

bool svg_item_use::update_children_tree ()
{
  const svg_attribute_xlink_href *href = get_computed_attribute<svg_attribute_xlink_href> ();
  abstract_svg_item *item_ref = href->href ();
  if (!item_ref)
    return false;

  insert_child (nullptr, item_ref->create_clone ());

  return true;
}
