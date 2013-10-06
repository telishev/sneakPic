#include "svg/items/svg_item_use.h"

#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attribute_type.h"

#include "svg/simple_item_observer.h"
#include "svg/svg_document.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"

#include "renderer/renderer_item_group.h"

class use_item_watcher : public simple_item_observer<use_item_watcher>
{
  QTransform m_transform_before;
public:
  use_item_watcher (svg_items_container *container, const std::string &parent)
    : simple_item_observer (container, parent) {}

  virtual void attribute_change_start (const std::string &/*sender*/, const abstract_attribute *attribute) override
  {
    if (!attribute || attribute->type () != svg_attribute_type::TRANSFORM)
      return;

    const svg_attribute_transform *atribute_transform = static_cast<const svg_attribute_transform *> (attribute);
    m_transform_before = atribute_transform->computed_transform ();
  }

  virtual void attribute_change_end (const std::string &/*sender*/, const abstract_attribute *attribute) override
  {
    if (!attribute || attribute->type () != svg_attribute_type::TRANSFORM)
      return;

    const svg_attribute_transform *atribute_transform = static_cast<const svg_attribute_transform *> (attribute);
    base_item()->base_transform_changed (m_transform_before, atribute_transform->computed_transform ());
    m_transform_before = atribute_transform->computed_transform ();
  }

  virtual void item_removed (const std::string &/*sender*/) override
  {
    base_item ()->unlink ();
  }

private:
  svg_item_use *base_item () const
  {
    return static_cast<svg_item_use *>(parent ());
  }

};

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
  auto href = get_computed_attribute<svg_attribute_xlink_href> ();
  auto x = get_computed_attribute<svg_attribute_x> ();
  auto y = get_computed_attribute<svg_attribute_y> ();
  auto transform = get_attribute_for_change<svg_attribute_transform> ();
  transform->set_additional_transform (QTransform::fromTranslate (x->value (), y->value ()));

  abstract_svg_item *item_ref = document ()->item_container ()->get_item (href->get_fragment_name ());
  if (item_ref)
    {
      abstract_svg_item *clone = item_ref->create_clone ();
      push_back (clone);
      observe_item (clone, new use_item_watcher (document ()->item_container (), name ()));
    }

  return true;
}

abstract_renderer_item *svg_item_use::create_renderer_item_impl () const
{
  renderer_item_group *renderer_item = new renderer_item_group (name ());
  update_group_item (renderer_item);
  return renderer_item;
}

void svg_item_use::unlink ()
{
  /// TODO:implement unlink
}

void svg_item_use::base_transform_changed (const QTransform &before, const QTransform &after)
{
  auto transform_attr = get_attribute_for_change<svg_attribute_transform> ();
  QTransform base_transform_change = before.inverted () * after;
  transform_attr->set_transform (transform_attr->computed_transform () * base_transform_change.inverted ());
}

bool svg_item_use::can_be_selected () const 
{
  return true;
}


