#include "svg/items/svg_item_use.h"

#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attribute_transform.h"

#include "renderer/renderer_item_group.h"

svg_item_use::svg_item_use (svg_document *document)
  : svg_item_group_type (document)
{
  m_renderer_item = nullptr;
}

svg_item_use::~svg_item_use ()
{
  FREE (m_renderer_item);
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

  abstract_svg_item *item_ref = href->href ();
  if (item_ref)
    insert_child (nullptr, item_ref->create_clone ());

  return true;
}

void svg_item_use::update_renderer_item ()
{
  if (!m_renderer_item)
    m_renderer_item = new renderer_item_group (this);

  update_group_item (m_renderer_item);
}

const abstract_renderer_item *svg_item_use::get_renderer_item () const 
{
  return m_renderer_item;
}
