#include "svg/undo/single_undo_item.h"

#include "svg/undo/undoable.h"

#include "svg/simple_state_diff.h"
#include "svg/undo/undoable_items_container.h"
#include "svg/undo/undo_test_common.h"


TEST (single_undo_item, undo_item_change)
{
  undoable_items_container_t container;
  single_undo_item undo_item (&container);
  undoable_stub *changed_item = new undoable_stub (1);
  container.add_item (changed_item);

  abstract_state_t *first_state = changed_item->create_state ();

  changed_item->m_data = 2;
  abstract_state_t *second_state = changed_item->create_state ();
  abstract_state_diff_t *diff = state_stub (0).create_diff (first_state, second_state);

  EXPECT_EQ (0, changed_item->undo_id ());
  EXPECT_EQ (2, changed_item->m_data);

  undo_item.add_item_diff (0, diff);

  undo_item.undo ();
  EXPECT_EQ (1, changed_item->m_data);

  undo_item.redo ();
  EXPECT_EQ (2, changed_item->m_data);
}


TEST (single_undo_item, undo_item_add)
{
  undoable_items_container_t container;
  single_undo_item undo_item (&container);

  abstract_state_t *first_state = NULL;
  undoable_stub *new_item = new undoable_stub (1);
  container.add_item (new_item);

  abstract_state_t *second_state = new_item->create_state ();
  abstract_state_diff_t *diff = state_stub (0).create_diff (first_state, second_state);

  EXPECT_EQ (0, new_item->undo_id ());
  undo_item.add_item_diff (0, diff);

  undo_item.undo ();
  EXPECT_EQ (nullptr, container.get_item (0));

  undo_item.redo ();
  undoable_stub *restored_item = dynamic_cast<undoable_stub *> (container.get_item (0));
  EXPECT_NE (nullptr, restored_item);
  EXPECT_EQ (1, restored_item->m_data);
}

TEST (single_undo_item, undo_item_remove)
{
  undoable_items_container_t container;
  single_undo_item undo_item (&container);

  undoable_stub *deleted_item = new undoable_stub (1);
  container.add_item (deleted_item);
  EXPECT_EQ (0, deleted_item->undo_id ());
  abstract_state_t *first_state = deleted_item->create_state ();
  container.remove_item (0);

  abstract_state_t *second_state = nullptr;
  abstract_state_diff_t *diff = state_stub (0).create_diff (first_state, second_state);
  
  undo_item.add_item_diff (0, diff);

  undo_item.undo ();
  undoable_stub *restored_item = dynamic_cast<undoable_stub *> (container.get_item (0));
  EXPECT_NE (nullptr, restored_item);
  EXPECT_EQ (1, restored_item->m_data);

  undo_item.redo ();
  EXPECT_EQ (nullptr, container.get_item (0));
}