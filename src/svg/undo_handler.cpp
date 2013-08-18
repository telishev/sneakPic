#include "svg/undo_handler.h"

#include "common/memory_deallocation.h"

#include "svg/single_undo_item_builder.h"
#include "svg/single_undo_item.h"
#include "svg/svg_document.h"


undo_handler::undo_handler (svg_document *document)
{
  m_document = document;
  m_builder = new single_undo_item_builder (m_document);
  m_cur_undo_position = 0;
}

undo_handler::~undo_handler ()
{
  FREE (m_builder);
}

void undo_handler::create_undo ()
{
  if ((int)m_undo_items.size () > m_cur_undo_position)
    m_undo_items.erase (m_undo_items.begin () + m_cur_undo_position, m_undo_items.end ());
  single_undo_item *item = m_builder->create_undo ();
  m_undo_items.push_back (std::unique_ptr<single_undo_item> (item));
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
  if (!m_document->signals_enabled ())
    return;

  m_builder->register_item (item);
}

void undo_handler::register_new_item (undoable *item)
{
  if (!m_document->signals_enabled ())
    return;

  m_builder->register_new_item (item);
}

void undo_handler::clear ()
{
  m_builder->clear ();
}
