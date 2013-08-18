#ifndef SINGLE_UNDO_ITEM_BUILDER_H
#define SINGLE_UNDO_ITEM_BUILDER_H

#include <unordered_map>
#include <memory>

class abstract_state_t;
class single_undo_item;
class undoable;
class svg_document;

class single_undo_item_builder
{
  svg_document *m_document;
  std::unordered_map<int, std::unique_ptr<abstract_state_t>> m_changed_items;
public:
  single_undo_item_builder (svg_document *document);
  ~single_undo_item_builder ();

  single_undo_item *create_undo ();

  void register_item (undoable *item);
  void register_new_item (undoable *item);

  void clear ();
};

#endif // SINGLE_UNDO_ITEM_BUILDER_H
