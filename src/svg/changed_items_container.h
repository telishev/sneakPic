#ifndef CHANGED_ITEMS_CONTAINER_H
#define CHANGED_ITEMS_CONTAINER_H

#include "svg/svg_item_observer.h"

#include <unordered_set>

class event_items_changed;
class svg_document;
class svg_graphics_item;

class changed_items_container : svg_item_observer
{
  std::unordered_set<std::string> m_changed_items;
  std::unordered_set<std::string> m_layout_changed_items;
  std::unordered_set<std::string> m_removed_items;

  svg_document *m_document;
public:
  changed_items_container (svg_document *document);
  ~changed_items_container ();

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

private:
  void clear ();
  void invalidate_bbox (const std::string &item_name);
  svg_graphics_item *get_graphics_item (const std::string &item_name) const;
};

#endif // CHANGED_ITEMS_CONTAINER_H

