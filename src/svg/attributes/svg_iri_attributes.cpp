#include "svg_iri_attributes.h"

#include "svg/svg_item_observer.h"
#include "svg/simple_item_observer.h"
#include "svg/changed_items_container.h"
#include "svg/undo/undo_handler.h"
#include "svg/items/abstract_svg_item.h"

class iri_observer : public simple_item_observer<iri_observer>
{
  svg_attribute_type m_type;
public:
  iri_observer (svg_items_container *container, const string &parent, svg_attribute_type type)
    : simple_item_observer (container, parent) { m_type = type; }

  virtual void item_changed (const string &/*sender*/, item_change_type /*type*/) override
  {
    if (parent ())
      parent ()->signal_item_changed (item_change_type::MYSELF, false);
  }
  svg_attribute_type type () const { return m_type; }

};

void svg_iri_attribute::set_iri (const svg_data_type_iri &iri)
{
  m_iri = iri;
  update_observers ();
}

void svg_iri_attribute::update_observers ()
{
  remove_observers ();
  if (!item () || m_iri.get_type () != iri_type::document_fragment)
    return;

  std::string fragment = m_iri.get_fragment_name ();
  abstract_svg_item *frag_item = get_item (fragment);
  if (frag_item)
    item ()->observe_item (frag_item, new iri_observer (get_container (), item ()->name (), type ()));
}

void svg_iri_attribute::remove_observers ()
{
  if (!get_edit_handler () || !item ())
    return;

  auto observers = item ()->get_created_observers ();
  if (!observers)
    return;

  undo_handler *handler = get_edit_handler ()->get_undo_handler ();

  for (int id : *observers)
  {
    undoable *observer = handler->get_item (id);
    auto my_observer = dynamic_cast<iri_observer *> (observer);
    if (!my_observer || my_observer->type () != type ())
      continue;

    item ()->erase_created_observer (my_observer);
  }
}
