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
#include "svg_item_type.h"

class use_item_watcher : public simple_item_observer<use_item_watcher>
{
  QTransform m_transform_before;
  unique_ptr<attribute_pointer<svg_attribute_transform>> m_transform;
public:
  use_item_watcher (svg_items_container *container, const string &parent)
    : simple_item_observer (container, parent) {}

  virtual void attribute_change_start (const string &/*sender*/, const abstract_attribute *attribute) override
  {
    if (!attribute || attribute->type () != svg_attribute_type::TRANSFORM)
      return;

    put_in (m_transform, parent ()->get_attribute_for_change<svg_attribute_transform> ());
    m_transform_before = full_chain_transform ();
  }

  virtual void attribute_change_end (const string &/*sender*/, const abstract_attribute *attribute) override
  {
    if (!attribute || attribute->type () != svg_attribute_type::TRANSFORM)
      return;

    QTransform transform = full_chain_transform ();
    QTransform base_transform_change = m_transform_before.inverted () * transform;
    (*m_transform)->set_transform ((*m_transform)->computed_transform () * base_transform_change.inverted ());
    m_transform.reset ();
    m_transform_before = transform;
  }

  virtual void item_removed (const string &/*sender*/) override
  {
    base_item ()->unlink ();
  }

  QTransform full_chain_transform () const
  {
    const abstract_svg_item *cur_item = base_item ();
    QTransform transfrom;
    while (cur_item->type () == svg_item_type::USE)
      {
        transfrom = transfrom * cur_item->get_computed_attribute<svg_attribute_transform> ()->computed_transform () ;
        cur_item = cur_item->child (0)->get_original_item ();
      }

    transfrom = cur_item->get_computed_attribute<svg_attribute_transform> ()->computed_transform () * transfrom;
    return transfrom;
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

bool svg_item_use::process_item_after_read ()
{
  return update_children_tree ();
}

bool svg_item_use::update_children_tree ()
{
  auto href = get_computed_attribute<svg_attribute_xlink_href> ();
  {
    auto x = get_attribute_for_change<svg_attribute_x> ();
    auto y = get_attribute_for_change<svg_attribute_y> ();
    auto transform = get_attribute_for_change<svg_attribute_transform> ();
    transform->set_additional_transform (QTransform::fromTranslate (x->value (), y->value ()));
    x->set_value (0);
    y->set_value (0);
  }

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
  unlink_item (child (0));
}


bool svg_item_use::can_be_selected () const 
{
  return true;
}

void svg_item_use::unlink_item (abstract_svg_item *item)
{
  if (!item)
    return;

  for (int i = 0; i < children_count (); i++)
    unlink_item (child (i));
}


