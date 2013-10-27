#include "svg/undo/single_undo_item_builder.h"

#include "svg/undo/undoable.h"
#include "svg/undo/undoable_items_container.h"
#include "svg/undo/single_undo_item.h"




single_undo_item_builder::single_undo_item_builder (undoable_items_container_t *items_container)
{
  m_items_container = items_container;
}

single_undo_item_builder::~single_undo_item_builder ()
{

}

single_undo_item *single_undo_item_builder::create_undo ()
{
  single_undo_item *single_item = new single_undo_item (m_items_container);
  for (auto &pair : m_changed_items)
    {
      int id = pair.first;
      abstract_state_t *old_state = pair.second.get ();

      undoable *item = m_items_container->get_item (id);
      abstract_state_t *new_state = item ? item->create_state () : nullptr;
      if (!old_state && !new_state)
        continue;

      abstract_state_diff_t *diff = old_state ? old_state->create_diff (old_state, new_state) : new_state->create_diff (old_state, new_state);
      single_item->add_item_diff (id, diff);
    }

  m_changed_items.clear ();
  return single_item;
}

void single_undo_item_builder::register_item (undoable *item)
{  
  int id = item->undo_id ();
  if (m_changed_items.find (id) != m_changed_items.end ())
    return;

  m_changed_items[id].reset (item->create_state ());
}

void single_undo_item_builder::register_new_item (undoable *item)
{
  int id = item->undo_id ();
  m_changed_items[id].reset (nullptr);
}

void single_undo_item_builder::clear ()
{
  m_changed_items.clear ();
}
