#ifndef UNDO_HANDLER_H
#define UNDO_HANDLER_H

#include <vector>
#include <memory>

class single_undo_item_builder;
class single_undo_item;
class undoable;
class undoable_items_container_t;

class undo_handler
{
  undoable_items_container_t *m_items_container;
  single_undo_item_builder *m_builder;
  vector<unique_ptr<single_undo_item>> m_undo_items;
  int m_cur_undo_position;
  bool m_signals_enabled;
public:
  undo_handler ();
  ~undo_handler ();

  void create_undo (QString name);

  void undo (int positions);
  void redo (int positions);

  void register_item (undoable *item);

  void clear ();

  undoable *get_item (int id) const;
  int add_item (undoable *item);
  void remove_item (undoable *item);
  void remove_item (int id);
  int assign_id (undoable *item);

  void set_signals_enabled (bool enabled) { m_signals_enabled = enabled; }
  int cur_undo_position () { return m_cur_undo_position; }

  bool has_undo () const;
  bool has_redo () const;

  QString undo_name () const;
  QString redo_name () const;


private:
  void register_new_item (undoable *item);
};

#endif // UNDO_HANDLER_H
