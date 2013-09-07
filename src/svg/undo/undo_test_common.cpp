#include "svg/undo/undo_test_common.h"




undoable_stub::undoable_stub (int data)
{
  m_data = data;
}

undoable_stub::~undoable_stub ()
{

}

int state_stub::data () const
{
  return m_data;
}

state_stub * state_stub::clone () const
{
  return new state_stub (m_data);
}

abstract_state_diff_t * state_stub::create_diff (const abstract_state_t *first, const abstract_state_t *second)
{
  return new simple_state_diff<state_stub> (first, second);
}

state_stub::state_stub (int data)
{
  m_data = data;
}

state_stub::~state_stub ()
{

}

undoable *state_stub::create_new_item ()
{
  return new undoable_stub (0);
}

abstract_state_t *undoable_stub::create_state ()
{
  return new state_stub (m_data);
}

void undoable_stub::load_from_state (const abstract_state_t *state)
{
  if (!state)
    return;

  const state_stub *my_state = static_cast<const state_stub *> (state);
  m_data = my_state->data ();
}




