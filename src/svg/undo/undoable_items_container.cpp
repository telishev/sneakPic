#include "svg/undo/undoable_items_container.h"

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"

#include "svg/undo/undoable.h"
#include "svg/undo/undo_handler.h"


undoable_items_container_t::undoable_items_container_t ()
{
  m_next_id = 0;
}

undoable_items_container_t::~undoable_items_container_t ()
{

}

undoable *undoable_items_container_t::get_item (int id) const
{
  auto it = m_map.find (id);
  if (it != m_map.end ())
    return it->second;

  return nullptr;
}

int undoable_items_container_t::add_item (undoable *item)
{
  assign_id (item);
  DEBUG_ASSERT (m_map.insert (std::make_pair (item->undo_id (), item)).second == true);
  return item->undo_id ();
}

void undoable_items_container_t::remove_item (int id)
{
  auto it = m_map.find (id);
  if (it == m_map.end ())
    return;

  FREE (it->second);
  m_map.erase (id);
}

int undoable_items_container_t::assign_id (undoable *item)
{
  if (item->undo_id () == -1)
    item->set_undo_id (m_next_id++);

  return item->undo_id ();
}
