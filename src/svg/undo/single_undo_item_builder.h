#ifndef SINGLE_UNDO_ITEM_BUILDER_H
#define SINGLE_UNDO_ITEM_BUILDER_H

#include <unordered_map>
#include <memory>

class abstract_state_t;
class single_undo_item;
class undoable;
class undoable_items_container_t;

class single_undo_item_builder
{
  undoable_items_container_t *m_items_container;
  std::unordered_map<int, unique_ptr<abstract_state_t>> m_changed_items;
public:
  single_undo_item_builder (undoable_items_container_t *items_container);
  ~single_undo_item_builder ();

  single_undo_item *create_undo ();

  void register_item (undoable *item);
  void register_new_item (undoable *item);

  void clear ();
};

#endif // SINGLE_UNDO_ITEM_BUILDER_H
