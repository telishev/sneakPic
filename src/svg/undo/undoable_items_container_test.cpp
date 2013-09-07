#include "svg/undo/undoable_items_container.h"

#include "svg/undo/undoable.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class undoable_mock : public undoable
{
public:
  undoable_mock ()
  {
    EXPECT_CALL (*this, destroy ()).Times (1);
  }

  ~undoable_mock () { destroy (); }

  virtual abstract_state_t *create_state () override { return nullptr; }
  virtual void load_from_state (const abstract_state_t * /*state*/) override { return; }

  MOCK_METHOD0 (destroy, void());
};

TEST (undoable_items_container, add_remove)
{
  undoable_mock *mock = new undoable_mock;
  undoable_items_container_t container;
  EXPECT_EQ (0, container.add_item (mock));
  int mock_id = mock->undo_id ();
  EXPECT_EQ (0, mock_id);
  EXPECT_EQ (mock, container.get_item (mock_id));

  container.remove_item (mock->undo_id ());
  EXPECT_EQ (nullptr, container.get_item (mock_id));
}

TEST (undoable_items_container, assign)
{
  undoable_mock *mock1 = new undoable_mock;
  undoable_mock *mock2 = new undoable_mock;
  undoable_items_container_t container;
  EXPECT_EQ (0, container.assign_id (mock1));
  EXPECT_EQ (1, container.add_item (mock2));
  EXPECT_EQ (0, container.add_item (mock1));
  EXPECT_EQ (mock1, container.get_item (0));
  EXPECT_EQ (mock2, container.get_item (1));
}

TEST (undoable_items_containerDeathTest, add_same_item)
{
  undoable_mock *mock = new undoable_mock;
  undoable_items_container_t container;
  EXPECT_EQ (0, container.add_item (mock));
  EXPECT_DEATH (container.add_item (mock), ".*");
  container.remove_item (0);
  container.remove_item (0);
}
