#include "editor/operations/transform_item_operation.h"

#include "svg/items/abstract_svg_item.h"

#include "svg/attributes/svg_attribute_transform.h"
#include "svg/items/svg_graphics_item.h"


transform_item_operation::transform_item_operation (svg_document *document)
{
  m_document = document;
}

transform_item_operation::~transform_item_operation ()
{

}

void transform_item_operation::apply_transform (const QTransform &new_transform, abstract_svg_item *item)
{
  item->get_attribute_for_change<svg_attribute_transform> ()->set_transform (get_new_transform (new_transform, item));
}

QTransform transform_item_operation::get_new_transform (const QTransform &new_transform, abstract_svg_item *item)
{
  auto transform = item->get_computed_attribute <svg_attribute_transform> ();
  QTransform par_comp_transform;
  if (item->parent () && item->parent ()->to_graphics_item ())
    par_comp_transform = item->parent ()->to_graphics_item ()->full_transform ();
  QTransform fixed_transform = par_comp_transform * new_transform * par_comp_transform.inverted ();
  return transform->computed_transform () * fixed_transform;
}
