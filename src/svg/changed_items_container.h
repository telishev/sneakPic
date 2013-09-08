#ifndef CHANGED_ITEMS_CONTAINER_H
#define CHANGED_ITEMS_CONTAINER_H

#include "svg/svg_item_observer.h"

#include <unordered_set>

class event_items_changed;
class svg_items_container;
class svg_graphics_item;
class undo_handler;

class items_edit_handler_t : public svg_item_observer
{
  std::unordered_set<std::string> m_changed_items;
  std::unordered_set<std::string> m_layout_changed_items;
  std::unordered_set<std::string> m_removed_items;

  svg_items_container *m_container;
  undo_handler        *m_undo_handler;
public:
  items_edit_handler_t (svg_items_container *container);
  ~items_edit_handler_t ();

  virtual void child_added (const std::string &parent, const std::string &child_name, int insert_pos) override;
  virtual void child_removed (const std::string &parent, const std::string &child_name, int pos) override;
  virtual void child_moved (const std::string &parent, const std::string &child_name, int old_pos, int new_pos) override;
  virtual void attribute_change_start (const std::string &sender, const abstract_attribute *computed_attribute) override;
  virtual void attribute_change_end (const std::string &sender, const abstract_attribute *computed_attribute) override;
  virtual void item_removed (const std::string &item) override;

  virtual abstract_state_t *create_state () override;
  virtual void load_from_state (const abstract_state_t *state) override;

  event_items_changed *create_changed_items_event ();

  void set_item_changed (const std::string &item);
  void set_item_layout_changed (const std::string &item);
  void set_item_removed (const std::string &item);

  undo_handler *get_undo_handler () const { return m_undo_handler; }

private:
  void clear ();
  void invalidate_bbox (const std::string &item_name);
  svg_graphics_item *get_graphics_item (const std::string &item_name) const;
};

#endif // CHANGED_ITEMS_CONTAINER_H

