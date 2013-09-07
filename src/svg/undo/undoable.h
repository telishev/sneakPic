#ifndef UNDOABLE_H
#define UNDOABLE_H

class abstract_state_diff_t;
class undoable;

class abstract_state_t
{
public:
  virtual ~abstract_state_t () = 0 {}
  virtual undoable *create_new_item () = 0;
  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) = 0;
};

enum class diff_direction_t
{
  UNDO,
  REDO,
};

class abstract_state_diff_t
{
public:
  virtual ~abstract_state_diff_t () = 0 {} 
  virtual abstract_state_t *apply_diff (const abstract_state_t *state, diff_direction_t direction) = 0;
};

class undoable
{
  int m_undo_id;
public:
  undoable () : m_undo_id (-1) {}

  virtual ~undoable () = 0 {}

  virtual abstract_state_t *create_state () = 0;
  virtual void load_from_state (const abstract_state_t *state) = 0;
  
  int undo_id () const { return m_undo_id; }
  void set_undo_id (int id) { m_undo_id = id; }

};

#endif // UNDOABLE_H
