#include "svg/undo/undo_handler.h"

#include "common/memory_deallocation.h"

#include "svg/undo/single_undo_item_builder.h"
#include "svg/undo/single_undo_item.h"
#include "svg/undo/undoable_items_container.h"
#include "svg/undo/undoable.h"


undo_handler::undo_handler ()
{
  m_items_container = new undoable_items_container_t;
  m_builder = new single_undo_item_builder (m_items_container);
  m_cur_undo_position = 0;
  m_signals_enabled = true;
}

undo_handler::~undo_handler ()
{
  FREE (m_builder);
  FREE (m_items_container);
}

void undo_handler::create_undo (QString name)
{
  if ((int)m_undo_items.size () > m_cur_undo_position)
    m_undo_items.erase (m_undo_items.begin () + m_cur_undo_position, m_undo_items.end ());
  single_undo_item *item = m_builder->create_undo ();
  item->set_name (name);
  m_undo_items.push_back (unique_ptr<single_undo_item> (item));
  m_cur_undo_position++;
}

void undo_handler::undo (int positions)
{
  positions = std::min (positions, m_cur_undo_position);
  for (int i = 0; i < positions; i++)
    {
      m_cur_undo_position--;
      single_undo_item *item = m_undo_items[m_cur_undo_position].get ();
      item->undo ();
    }

  m_builder->clear ();
}

void undo_handler::redo (int positions)
{
  positions = std::min (positions, (int)m_undo_items.size () - m_cur_undo_position);
  for (int i = 0; i < positions; i++)
    {
      single_undo_item *item = m_undo_items[m_cur_undo_position].get ();
      item->redo ();
      m_cur_undo_position++;
    }

  m_builder->clear ();
}

void undo_handler::register_item (undoable *item)
{
  if (!m_signals_enabled)
    return;

  m_builder->register_item (item);
}

void undo_handler::clear ()
{
  m_builder->clear ();
}

undoable *undo_handler::get_item (int id) const
{
  return m_items_container->get_item (id);
}

int undo_handler::add_item (undoable *item)
{
  int id = m_items_container->add_item (item);
  m_builder->register_new_item (item);
  return id;
}

void undo_handler::remove_item (undoable *item)
{
  if (!item)
    return;

  register_item (item);
  m_items_container->remove_item (item->undo_id ());
}

void undo_handler::remove_item (int id)
{
  undoable *item = m_items_container->get_item (id);
  return remove_item (item);
}

int undo_handler::assign_id (undoable *item)
{
  return m_items_container->assign_id (item);
}

void undo_handler::register_new_item (undoable *item)
{
  if (!m_signals_enabled)
    return;

  m_builder->register_new_item (item);
}

bool undo_handler::has_undo () const
{
  return m_cur_undo_position != 0;
}

bool undo_handler::has_redo () const
{
  return m_cur_undo_position < (int)m_undo_items.size ();
}

QString undo_handler::undo_name () const
{
  if (!has_undo ())
    return QString ();

  return m_undo_items[m_cur_undo_position - 1]->name ();
}

QString undo_handler::redo_name () const
{
  if (!has_redo ())
    return QString ();

  return m_undo_items[m_cur_undo_position]->name ();
}
