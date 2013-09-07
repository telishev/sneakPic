#ifndef UNDO_HANDLER_H
#define UNDO_HANDLER_H

#include <vector>
#include <memory>

class single_undo_item_builder;
class single_undo_item;
class svg_document;
class undoable;
class undoable_items_container_t;

class undo_handler
{
  svg_document *m_document;
  undoable_items_container_t *m_items_container;
  single_undo_item_builder *m_builder;
  std::vector<std::unique_ptr<single_undo_item>> m_undo_items;
  int m_cur_undo_position;
public:
  undo_handler (svg_document *document);
  ~undo_handler ();

  void create_undo (); 

  void undo (int positions);
  void redo (int positions);

  void register_item (undoable *item);

  void clear ();

  undoable *get_item (int id) const;
  int add_item (undoable *item);
  void remove_item (undoable *item);
  void remove_item (int id);
  int assign_id (undoable *item);

private:
  void register_new_item (undoable *item);
};

#endif // UNDO_HANDLER_H
