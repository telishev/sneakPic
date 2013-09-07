#ifndef UNDOABLE_ITEMS_CONTAINER_H
#define UNDOABLE_ITEMS_CONTAINER_H

#include <unordered_map>

class undoable;

class undoable_items_container_t
{
  std::unordered_map<int, undoable *> m_map;
  int m_next_id;

public:
  undoable_items_container_t ();
  ~undoable_items_container_t ();

  undoable *get_item (int id) const;
  int add_item (undoable *item);
  void remove_item (int id);
  int assign_id (undoable *item);
};

#endif // UNDOABLE_ITEMS_CONTAINER_H
