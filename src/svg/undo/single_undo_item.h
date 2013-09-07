#ifndef SINGLE_UNDO_ITEM_H
#define SINGLE_UNDO_ITEM_H

#include <vector>
#include <utility>

class abstract_state_diff_t;
class undoable_items_container_t;
enum class diff_direction_t;

class single_undo_item
{
  struct item_and_diff
  {
    int m_id;
    abstract_state_diff_t *m_diff;

    item_and_diff (int id, abstract_state_diff_t *diff);
    item_and_diff (item_and_diff &&rhs);
    ~item_and_diff ();
  private:
    item_and_diff (const item_and_diff &rhs);
  };

  undoable_items_container_t *m_items_container;
  std::vector<item_and_diff> m_diff_map;
public:
  single_undo_item (undoable_items_container_t *items_container);
  ~single_undo_item ();

  void undo ();
  void redo ();

  void add_item_diff (int id, abstract_state_diff_t *diff);

private:
  void apply (diff_direction_t direction);
};

#endif // SINGLE_UNDO_ITEM_H
