#ifndef SIMPLE_ITEM_OBSERVER_H
#define SIMPLE_ITEM_OBSERVER_H

#include "svg/svg_item_observer.h"
#include "svg/simple_state_diff.h"
#include "svg/svg_document.h"
#include "svg/items/svg_items_container.h"

class svg_document;


template<typename T>
class simple_observer_state_t : public abstract_state_t
{
  svg_document *m_document;
  std::string m_parent;
  int m_undo_id;
public:
  simple_observer_state_t (svg_document *document, const std::string &parent, int undo_id)
    : m_document (document), m_parent (parent), m_undo_id (undo_id) {}

  virtual undoable *create_new_item () override
  {
    undoable *item = new simple_item_observer<T> (m_document, m_parent);
    item->set_undo_id (m_undo_id);
    return item;
  }

  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override
  {
    return new simple_state_diff<simple_observer_state_t<T>> (first, second);
  }

  simple_observer_state_t *clone () const
  {
    return new simple_observer_state_t (m_document, m_parent, m_undo_id);
  }
};

template<typename T>
class simple_item_observer : public svg_item_observer
{
  svg_document *m_document;
  std::string m_parent;
public:
  simple_item_observer (svg_document *document, const std::string &parent)
    : m_document (document), m_parent (parent) {}
  ~simple_item_observer () {}

  virtual abstract_state_t *create_state () override
  {
    return new simple_observer_state_t<T> (m_document, m_parent, undo_id ());
  }

  virtual void load_from_state (const abstract_state_t * /*state*/) override {}

  svg_document *document () const { return m_document; }
  abstract_svg_item *parent () const
  {
    return m_document->item_container ()->get_item (m_parent);
  }

  virtual void item_removed (const std::string &/*item*/) override
  {
    parent ()->erase_created_observer (this);
  }

};

#endif // SIMPLE_ITEM_OBSERVER_H
