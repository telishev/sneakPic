#include "svg/items/svg_item_use.h"

#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attribute_transform.h"

#include "renderer/renderer_item_group.h"

svg_item_use::svg_item_use (svg_document *document)
  : svg_item_group_type (document)
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
  const svg_attribute_x *x = get_computed_attribute<svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  svg_attribute_transform *transform = get_attribute<svg_attribute_transform> ();
  if (!transform)
    {
      transform = new svg_attribute_transform (this);
      add_attribute (transform);
    }

  transform->set_additional_transform (QTransform::fromTranslate (x->value (), y->value ()));

  abstract_svg_item *item_ref = href->get_fragment ();
  if (item_ref)
    insert_child (nullptr, item_ref->create_clone ());

  return true;
}

abstract_renderer_item *svg_item_use::create_renderer_item () const
{
  renderer_item_group *renderer_item = new renderer_item_group (id ().toStdString ());
  update_group_item (renderer_item);
  return renderer_item;
}
