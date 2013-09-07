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



svg_document::svg_document (settings_t *settings)
{
  m_item_factory = new svg_item_factory (this);
  m_attribute_factory = new svg_attribute_factory (this);
  m_item_container = new svg_items_container;
  m_selectors = new selectors_container;
  m_changed_items = new changed_items_container (m_item_container);
  m_undo_handler = new undo_handler;
  m_root = nullptr;
  m_last_overlay_num = 0;
  m_settings = settings;
  m_queue = nullptr;
  set_signals_enabled (false);
}

svg_document::~svg_document ()
{
  FREE (m_item_factory);
  FREE (m_attribute_factory);
  FREE (m_item_container);
  FREE (m_selectors);
  FREE (m_changed_items);
  FREE (m_undo_handler);
}

static inline QString get_namespace_name (const QXmlStreamNamespaceDeclarations &declarations, const QString &uri)
{
  for (int i = 0; i < declarations.size (); i++)
    if (declarations[i].namespaceUri () == uri)
      return declarations[i].prefix ().toString ();

  return QString ();
}

bool svg_document::read_file (const QString &filename_arg)
{
  m_filename = filename_arg;

  QFile file (m_filename);
  if (!file.open (QIODevice::ReadOnly))
    return false;


  QXmlStreamReader reader (&file);
  abstract_svg_item *cur_item = nullptr;
  while (!reader.atEnd ())
    {
      switch (reader.readNext ())
        {
          case QXmlStreamReader::Invalid:
          case QXmlStreamReader::NoToken:
            return false;
          case QXmlStreamReader::Comment:
          case QXmlStreamReader::DTD:
          case QXmlStreamReader::EndDocument:
          case QXmlStreamReader::EntityReference:
          case QXmlStreamReader::ProcessingInstruction:
          case QXmlStreamReader::StartDocument:
            break;

          case QXmlStreamReader::Characters:
            {
              if (!cur_item)
                DEBUG_PAUSE ("cur_item must not be nullptr");

              QString char_data = reader.text ().toString ().trimmed ();
              if (!char_data.isEmpty ())
                {
                  svg_character_data *data = new svg_character_data (this, reader.text ().toUtf8 ().constData ());
                  cur_item->push_back (data);
                }
              break;
            }
          case QXmlStreamReader::EndElement:
            {
              if (!cur_item)
                DEBUG_PAUSE ("cur_item must not be nullptr");

              cur_item->item_read_complete ();
              cur_item = cur_item->parent ();
              break;
            }
          case QXmlStreamReader::StartElement:
            {
              cur_item = process_new_item (reader, cur_item);
              break;
            }

        }
    }

  if (reader.hasError () || !m_root)
    return false;

  DEBUG_ASSERT (m_root->type () == svg_item_type::SVG);

  if (!m_root->check ())
    return false;

  svg_graphics_item *graphics_item = m_root->to_graphics_item ();
  if (!graphics_item)
    return false;

  m_item_container->set_root (m_root->name ());
  m_undo_handler->clear ();
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
  QFile file (m_filename);
  if (!file.open (QIODevice::WriteOnly))
    return false;

  QXmlStreamWriter writer (&file);
  writer.setAutoFormatting(true);
  writer.writeDefaultNamespace (svg_namespaces::svg_uri ());
  std::map<QString, QString> namespaces;
  m_root->get_used_namespaces (namespaces);
  for (auto namespace_pair : namespaces)
    writer.writeNamespace (namespace_pair.first, namespace_pair.second);
  writer.writeStartDocument();
  m_root->write (writer);
  writer.writeEndDocument();
  return false;
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

abstract_svg_item *svg_document::process_new_item (QXmlStreamReader &reader, abstract_svg_item *cur_item)
{
  QString namespace_uri = reader.namespaceUri ().toString ();
  QString name = reader.name ().toString ();
  QString namespace_name = get_namespace_name (reader.namespaceDeclarations (), namespace_uri);
  abstract_svg_item *child_item = m_item_factory->create_item (name, namespace_uri, namespace_name);
  if (cur_item)
    cur_item->push_back (child_item);
  else
    {
      m_undo_handler->add_item (child_item);
      m_root = child_item;
    }

  QXmlStreamAttributes attributes = reader.attributes ();

  for (int i = 0; i < attributes.size (); i++)
    {
      QStringRef attribute_namespace_uri = attributes[i].namespaceUri ();
      QStringRef attribute_name = attributes[i].name ();
      QStringRef attribute_namespace_name = attributes[i].prefix ();
      abstract_attribute *attribute = m_attribute_factory->create_attribute (child_item,
        attribute_name.toLatin1 ().constData (), attribute_namespace_uri.toLatin1 ().constData (), attribute_namespace_name.toLatin1 ().constData ());

      if (attribute->inherit_type () != svg_inherit_type::NONE && attributes[i].value () == "inherit")
        attribute->set_is_inherited (true);

      if (  attribute->read (attributes[i].value ().toLatin1 ().constData ())
          || attribute->type () == svg_attribute_type::UNKNOWN)
        child_item->add_attribute (attribute);
      else
        FREE (attribute);
    }

  child_item->process_after_read ();
  return child_item;
}

QString svg_document::create_overlay_name ()
{
  return QString ("#overlay%1").arg (m_last_overlay_num++);
}

void svg_document::apply_changes ()
{
  if (!m_queue)
    return;

  m_undo_handler->create_undo ();
  send_items_change ();
}

bool svg_document::signals_enabled () const
{
  return m_signals_enabled;
}

void svg_document::set_signals_enabled (bool enable)
{
  m_signals_enabled = enable;
  m_undo_handler->set_signals_enabled (enable);
}

void svg_document::undo ()
{
  m_undo_handler->undo (1);
  send_items_change ();
}

void svg_document::redo ()
{
  m_undo_handler->redo (1);
  send_items_change ();
}

void svg_document::send_items_change ()
{
  m_queue->add_event_and_wait (m_changed_items->create_changed_items_event ());
  emit items_changed ();
}
