#include "svg/svg_document.h"

#include "common/memory_deallocation.h"
#include "common/debug_utils.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_type.h"
#include "svg/attributes/svg_attribute_factory.h"

#include "svg/items/svg_item_factory.h"
#include "svg/items/svg_item_type.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/items/svg_character_data.h"

#include "svg/copy_paste_handler.h"

#include "svg/css/selectors_container.h"

#include "svg/svg_namespaces.h"
#include "svg/undo/undo_handler.h"
#include "svg/changed_items_container.h"

#include "renderer/renderer_items_container.h"
#include "renderer/abstract_renderer_item.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/events_queue.h"
#include "renderer/event_items_changed.h"
#include "svg_reader.h"
#include "svg_writer.h"
#include "document_change_sender.h"



svg_document::svg_document ()
{
  m_item_factory = new svg_item_factory (this);
  m_item_container = new svg_items_container;
  m_items_edit_handler = new items_edit_handler_t (m_item_container);
  m_root = nullptr;
  m_queue = nullptr;
  m_change_sender = nullptr;
  set_signals_enabled (false);
}

svg_document::~svg_document ()
{
  FREE (m_item_factory);
  FREE (m_item_container);
  FREE (m_items_edit_handler);
  FREE (m_change_sender);
}

bool svg_document::read_file (const QString &filename_arg)
{
  m_filename = filename_arg;

  svg_reader reader (get_undo_handler (), m_item_factory, this);
  if (!reader.read_file (m_filename))
    return false;

  return finalize_doc_creation (reader);

}

QString svg_document::get_filename ()
{
  return m_filename.isEmpty () ? "New Document" : m_filename;
}

bool svg_document::is_new_document ()
{
  return m_filename.isEmpty ();
}

bool svg_document::write_file (const QString &filename_arg)
{
  m_filename = filename_arg;
  svg_writer writer;
  return writer.write_to_file (filename_arg, m_root);
}

renderer_items_container *svg_document::create_rendered_items (rendered_items_cache *cache)
{
  renderer_items_container *renderer_items = new renderer_items_container;
  renderer_items->set_cache (cache);
  if (cache)
    cache->clear_selection_mapping ();

  create_renderer_item (renderer_items, m_root);
  renderer_items->set_root (m_root->name ());
  renderer_items->root ()->update_bbox ();
  return renderer_items;
}

void svg_document::create_renderer_item (renderer_items_container *renderer_items, abstract_svg_item *svg_item)
{
  svg_graphics_item *graphics_item = svg_item->to_graphics_item ();
  if (!graphics_item)
    return;
  abstract_renderer_item *renderer_item = graphics_item->create_renderer_item ();
  if (!renderer_item)
    return;

  renderer_items->add_item (renderer_item);

  for (int i = 0; i < svg_item->children_count (); i++)
    create_renderer_item (renderer_items, svg_item->child (i));
}

void svg_document::apply_changes (QString name)
{
  if (!m_queue)
    return;

  get_undo_handler ()->create_undo (name);
  emit changes_done ();
  update_renderer ();
}

bool svg_document::signals_enabled () const
{
  return m_signals_enabled;
}

void svg_document::set_signals_enabled (bool enable)
{
  m_signals_enabled = enable;
  get_undo_handler ()->set_signals_enabled (enable);
}

void svg_document::undo ()
{
  get_undo_handler ()->undo (1);
  update_renderer ();
  emit undo_redo_done ();
}

void svg_document::redo ()
{
  get_undo_handler ()->redo (1);
  update_renderer ();
  emit undo_redo_done ();
}

undo_handler *svg_document::get_undo_handler () const
{
  return m_items_edit_handler->get_undo_handler ();
}

void svg_document::redraw ()
{
  if (m_change_sender)
    m_change_sender->set_redraw_needed ();
  emit items_changed ();
}

void svg_document::set_queue (events_queue *queue)
{
  m_queue = queue;
  m_change_sender = new document_change_sender (m_queue, m_items_edit_handler);
}

void svg_document::update_renderer ()
{
  if (m_change_sender)
    m_change_sender->update ();
  emit items_changed ();
}

void svg_document::process_new_item (abstract_svg_item *item)
{
  get_undo_handler ()->add_item (item);
  item->update_name ();
}

bool svg_document::create_new_document ()
{
  svg_reader reader (get_undo_handler (), m_item_factory, this);
  if (!reader.create_new_document ())
    return false;

  return finalize_doc_creation (reader);
}

bool svg_document::finalize_doc_creation (svg_reader &reader)
{
  m_root = reader.root ();
  if (!m_root)
    return false;

  DEBUG_ASSERT (m_root->type () == svg_item_type::SVG);

  if (!m_root->process_after_read ())
    return false;

  svg_graphics_item *graphics_item = m_root->to_graphics_item ();
  if (!graphics_item)
    return false;

  m_item_container->set_root (m_root->name ());
  get_undo_handler ()->clear ();
  graphics_item->update_bbox ();
  set_signals_enabled (true);
  return true;
}

