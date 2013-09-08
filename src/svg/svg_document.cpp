#include "svg/svg_document.h"

#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

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



svg_document::svg_document ()
{
  m_item_factory = new svg_item_factory (this);
  m_item_container = new svg_items_container;
  m_items_edit_handler = new items_edit_handler_t (m_item_container);
  m_root = nullptr;
  m_last_overlay_num = 0;
  m_queue = nullptr;
  set_signals_enabled (false);
}

svg_document::~svg_document ()
{
  FREE (m_item_factory);
  FREE (m_item_container);
  FREE (m_items_edit_handler);
}

bool svg_document::read_file (const QString &filename_arg)
{
  m_filename = filename_arg;

  svg_reader reader (get_undo_handler (), m_item_factory, this);
  if (!reader.read_file (m_filename))
    return false;

  m_root = reader.root ();
  if (!m_root)
    return false;

  DEBUG_ASSERT (m_root->type () == svg_item_type::SVG);

  if (!m_root->check ())
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

QString svg_document::get_filename ()
{
  return m_filename;
}

bool svg_document::write_file (const QString &filename_arg)
{
  m_filename = filename_arg;
  svg_writer writer (m_root);
  return writer.write (filename_arg);
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

std::string svg_document::create_overlay_name ()
{
  char buf[32];
  return std::string ("#overlay") + itoa (m_last_overlay_num++, buf, 10);
}

void svg_document::apply_changes ()
{
  if (!m_queue)
    return;

  get_undo_handler ()->create_undo ();
  send_items_change ();
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
  send_items_change ();
}

void svg_document::redo ()
{
  get_undo_handler ()->redo (1);
  send_items_change ();
}

void svg_document::send_items_change ()
{
  m_queue->add_event_and_wait (m_items_edit_handler->create_changed_items_event ());
  emit items_changed ();
}

undo_handler *svg_document::get_undo_handler () const
{
  return m_items_edit_handler->get_undo_handler ();
}
