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
  T m_state;
public:
  simple_observer_state_t (const T &value) : m_state (value) {}

  virtual undoable *create_new_item () override
  {
    return new T (m_state);
  }

  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override
  {
    return new simple_state_diff<simple_observer_state_t<T>> (first, second);
  }

  simple_observer_state_t *clone () const
  {
    return new simple_observer_state_t (m_state);
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

  virtual abstract_state_t *create_state () override
  {
    return new simple_observer_state_t<T> (*static_cast<T *> (this));
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
