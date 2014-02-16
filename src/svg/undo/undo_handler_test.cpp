#include "svg/undo/undo_handler.h"

#include "svg/undo/undo_test_common.h"


TEST (undo_handler, test_change)
{
  undo_handler handler;
  undoable_stub *change_item = new undoable_stub (1);
  handler.add_item (change_item);
  handler.clear ();
  handler.register_item (change_item);
  change_item->m_data = 2;
  handler.create_undo ("Test");
  
  handler.register_item (change_item);
  change_item->m_data = 3;
  handler.create_undo ("Test");

  handler.register_item (change_item);
  change_item->m_data = 4;
  handler.create_undo ("Test");

  handler.undo (2);
  EXPECT_EQ (2, change_item->m_data);

  handler.register_item (change_item);
  change_item->m_data = 5;
  handler.create_undo ("Test");

  handler.undo (1);
  EXPECT_EQ (2, change_item->m_data);

  handler.undo (1);
  EXPECT_EQ (1, change_item->m_data);

  handler.undo (1);
  EXPECT_EQ (1, change_item->m_data);

  handler.redo (2);
  EXPECT_EQ (5, change_item->m_data);

  handler.undo (1);
  EXPECT_EQ (2, change_item->m_data);
}

static inline void check_item (undo_handler &handler, int index, int value)
{
  undoable_stub *item = dynamic_cast<undoable_stub *>(handler.get_item (index));
  EXPECT_NE (nullptr, item);
  EXPECT_EQ (value, item->m_data);
}

TEST (undo_handler, test_add_remove)
{
  undo_handler handler;
  undoable_stub *new_item1 = new undoable_stub (1);
  handler.add_item (new_item1);
  EXPECT_EQ (0, new_item1->undo_id ());
  handler.create_undo ("Test"); // 1
  
  handler.remove_item (new_item1);
  handler.create_undo ("Test"); // 2

  undoable_stub *new_item2 = new undoable_stub (2);
  handler.add_item (new_item2);
  EXPECT_EQ (1, new_item2->undo_id ());
  handler.create_undo ("Test"); // 3

  handler.undo (1); // 2
  EXPECT_EQ (nullptr, handler.get_item (1));
  EXPECT_EQ (nullptr, handler.get_item (0));

  handler.undo (1); // 1
  EXPECT_EQ (nullptr, handler.get_item (1));
  check_item (handler, 0, 1);

  undoable_stub *new_item3 = new undoable_stub (3);
  handler.add_item (new_item3);
  handler.create_undo ("Test"); // 2

  handler.undo (1); // 1
  EXPECT_EQ (nullptr, handler.get_item (1));
  check_item (handler, 0, 1);
}

