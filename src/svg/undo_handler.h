#ifndef UNDO_HANDLER_H
#define UNDO_HANDLER_H

#include <vector>
#include <memory>

class single_undo_item_builder;
class single_undo_item;
class svg_document;
class undoable;

class undo_handler
{
  svg_document *m_document;
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
  void register_new_item (undoable *item);

  void clear ();
};

#endif // UNDO_HANDLER_H
