#ifndef CHANGED_ITEMS_CONTAINER_H
#define CHANGED_ITEMS_CONTAINER_H

#include "svg/svg_item_observer.h"

#include <unordered_set>
#include <QObject>

class event_items_changed;
class svg_items_container;
class svg_graphics_item;
class undo_handler;
class abstract_svg_item;

class items_edit_handler_t : public QObject, public svg_item_observer
{
  Q_OBJECT

  std::unordered_set<string> m_changed_items;
  std::unordered_set<string> m_layout_changed_items;
  std::unordered_set<string> m_removed_items;

  svg_items_container *m_container;
  undo_handler        *m_undo_handler;
public:
  items_edit_handler_t (svg_items_container *container);
  ~items_edit_handler_t ();

  virtual void child_added (const string &parent, const string &child_name, int insert_pos) override;
  virtual void child_removed (const string &parent, const string &child_name, int pos) override;
  virtual void child_moved (const string &parent, const string &child_name, int old_pos, int new_pos) override;
  virtual void attribute_change_start (const string &sender, const abstract_attribute *computed_attribute) override;
  virtual void attribute_change_end (const string &sender, const abstract_attribute *computed_attribute) override;
  virtual void item_removed (const string &item) override;
  virtual void layout_changed (const string &sender) override;
  virtual void item_changed (const string &sender, item_change_type type) override;

  virtual abstract_state_t *create_state () override;
  virtual void load_from_state (const abstract_state_t *state) override;

  event_items_changed *create_changed_items_event (bool need_to_clear);

  void set_item_changed (const string &item);
  void set_item_layout_changed (const string &item);
  void set_item_removed (const string &item);

  undo_handler *get_undo_handler () const { return m_undo_handler; }

  abstract_svg_item *get_item (const std::string &name) const;
  svg_items_container *get_container () const { return m_container; }

signals:
  void child_added_signal (const string &parent, const string &child_name, int insert_pos);
  void child_removed_signal (const string &parent, const string &child_name, int pos);
  void child_moved_signal (const string &parent, const string &child_name, int old_pos, int new_pos);
  void attribute_change_start_signal (const string &sender, const abstract_attribute *computed_attribute);
  void attribute_change_end_signal (const string &sender, const abstract_attribute *computed_attribute);
  void item_removed_signal (const string &item);

private:

  void invalidate_bbox (const string &item_name);
  svg_graphics_item *get_graphics_item (const string &item_name) const;
  void set_children_changed (const string &parent_name);
  void clear ();
};

#endif // CHANGED_ITEMS_CONTAINER_H

