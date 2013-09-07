#ifndef UNDO_TEST_COMMON_H
#define UNDO_TEST_COMMON_H

#include "svg/undo/undoable.h"
#include "svg/simple_state_diff.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class undoable_stub : public undoable
{
public:
  int m_data;
  undoable_stub (int data);
  ~undoable_stub ();

  virtual abstract_state_t *create_state () override;
  virtual void load_from_state (const abstract_state_t *state) override;
};

class state_stub : public abstract_state_t
{
  int m_data;
public:
  state_stub (int data);
  virtual ~state_stub ();
  virtual undoable *create_new_item () override;
  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override;

  state_stub *clone () const;
  int data () const;
};


#endif // UNDO_TEST_COMMON_H
