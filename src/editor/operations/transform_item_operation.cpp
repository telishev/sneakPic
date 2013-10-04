#include "editor/operations/transform_item_operation.h"

#include "svg/items/abstract_svg_item.h"

#include "svg/attributes/svg_attribute_transform.h"


transform_item_operation::transform_item_operation (svg_document *document)
{
  m_document = document;
}

transform_item_operation::~transform_item_operation ()
{

}

void transform_item_operation::apply_transform (const QTransform &new_transform, abstract_svg_item *item)
{
  auto transform = item->get_attribute_for_change<svg_attribute_transform> ();
  transform->set_transform (transform->computed_transform () * new_transform);
}
