#ifndef SIMPLE_ITEM_OBSERVER_H
#define SIMPLE_ITEM_OBSERVER_H

#include "svg/svg_item_observer.h"
#include "svg/simple_state_diff.h"
#include "svg/items/svg_items_container.h"

class svg_items_container;

template<typename T>
class simple_item_observer;


template<typename T>
class simple_observer_state_t : public abstract_state_t
{
  svg_items_container *m_container;
  string m_parent;
public:
  simple_observer_state_t (svg_items_container *container, const string &parent)
    : m_container (container), m_parent (parent) {}

  virtual undoable *create_new_item () override
  {
    undoable *item = new T (m_container, m_parent);
    return item;
  }

  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override
  {
    return new simple_state_diff<simple_observer_state_t<T>> (first, second);
  }

  simple_observer_state_t *clone () const
  {
    return new simple_observer_state_t (m_container, m_parent);
  }
};

template<typename T>
class simple_item_observer : public svg_item_observer
{
  svg_items_container *m_container;
  string m_parent;
public:
  simple_item_observer (svg_items_container *container, const string &parent)
    : m_container (container), m_parent (parent) {}
  ~simple_item_observer () {}

  virtual abstract_state_t *create_state () override
  {
    return new simple_observer_state_t<T> (m_container, m_parent);
  }

  virtual void load_from_state (const abstract_state_t * /*state*/) override {}

  svg_items_container *container () const { return m_container; }
  abstract_svg_item *parent () const
  {
    return m_container->get_item (m_parent);
  }

  virtual void item_removed (const string &/*item*/) override
  {
    parent ()->erase_created_observer (this);
  }

};

#endif // SIMPLE_ITEM_OBSERVER_H
