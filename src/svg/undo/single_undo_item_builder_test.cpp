#include "svg/undo/single_undo_item_builder.h"

#include "svg/undo/undo_test_common.h"
#include "svg/undo/undoable_items_container.h"
#include "svg/undo/single_undo_item.h"

TEST (single_undo_item_builder, undo_item_change)
{
  undoable_items_container_t container;
  single_undo_item_builder builder (&container);
  undoable_stub *edited_item = new undoable_stub (1);
  EXPECT_EQ (0, container.add_item (edited_item));
  builder.register_item (edited_item);
  edited_item->m_data = 2;
  std::unique_ptr <single_undo_item> undo (builder.create_undo ());
  EXPECT_NE (nullptr, undo);
  undo->undo ();
  EXPECT_EQ (1, edited_item->m_data);
  undo->redo ();
  EXPECT_EQ (2, edited_item->m_data);
}

TEST (single_undo_item_builder, undo_item_add)
{
  undoable_items_container_t container;
  single_undo_item_builder builder (&container);
  undoable_stub *edited_item = new undoable_stub (1);
  EXPECT_EQ (0, container.add_item (edited_item));
  builder.register_new_item (edited_item);

  std::unique_ptr <single_undo_item> undo (builder.create_undo ());
  EXPECT_NE (nullptr, undo);

  undo->undo ();
  EXPECT_EQ (nullptr, container.get_item (0));

  undo->redo ();
  undoable_stub *restored_item = dynamic_cast<undoable_stub *> (container.get_item (0));
  EXPECT_NE (nullptr, restored_item);
  EXPECT_EQ (1, restored_item->m_data);
}

TEST (single_undo_item_builder, undo_item_remove)
{
  undoable_items_container_t container;
  single_undo_item_builder builder (&container);
  undoable_stub *edited_item = new undoable_stub (1);
  EXPECT_EQ (0, container.add_item (edited_item));
  builder.register_item (edited_item);
  container.remove_item (0);
  std::unique_ptr <single_undo_item> undo (builder.create_undo ());
  EXPECT_NE (nullptr, undo);

  undo->undo ();
  undoable_stub *restored_item = dynamic_cast<undoable_stub *> (container.get_item (0));
  EXPECT_NE (nullptr, restored_item);
  EXPECT_EQ (1, restored_item->m_data);

  undo->redo ();
  EXPECT_EQ (nullptr, container.get_item (0));
}