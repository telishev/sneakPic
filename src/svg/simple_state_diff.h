#ifndef SIMPLE_STATE_DIFF_H
#define SIMPLE_STATE_DIFF_H

#include "svg/undo/undoable.h"

template<typename T>
class simple_state_diff : public abstract_state_diff_t
{
  T *m_states[2];
public:
  simple_state_diff (const abstract_state_t *first, const abstract_state_t *second)
  {
    m_states[0] = first ? static_cast<const T *>(first)->clone () : nullptr;
    m_states[1] = second ? static_cast<const T *>(second)->clone () : nullptr;
  }
  ~simple_state_diff ()
  {
    delete m_states[0];
    delete m_states[1];
  }

  virtual abstract_state_t *apply_diff (const abstract_state_t * /*state*/, diff_direction_t direction) override
  {
    T *state = (direction == diff_direction_t::UNDO ? m_states[0] : m_states[1]);
    return state ? state->clone () : nullptr;
  }
};

#endif // SIMPLE_STATE_DIFF_H
