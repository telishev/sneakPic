#include "clone_item_operation.h"

#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_use.h"
#include "svg/svg_document.h"
#include "add_item_operation.h"



clone_item_operation::clone_item_operation (svg_painter *painter)
{
  m_painter = painter;
}


svg_item_use *clone_item_operation::apply (abstract_svg_item *item)
{
  if (!item->to_graphics_item ())
    return nullptr;

  QString base_name = QString::fromStdString (item->name ());
  auto new_item = item->document ()->create_new_svg_item<svg_item_use> ();
  new_item->get_attribute_for_change<svg_attribute_xlink_href> ()->iri ()->create_from_element (base_name);
  new_item->update_children_tree ();
  add_item_operation add_op (m_painter, false);
  add_op.set_apply_style (false);
  add_op.apply (new_item);
  return new_item;
}
