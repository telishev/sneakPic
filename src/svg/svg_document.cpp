#include "svg/svg_document.h"

#include <QFile>
#include <QDomDocument>
#include <QTextStream>

#include "common/memory_deallocation.h"

#include "svg/items/svg_item_factory.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_svg.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_type.h"

#include "svg/svg_namespaces.h"

#include "svg/attributes/svg_attribute_factory.h"

#include "renderer/renderer_items_container.h"
#include "renderer/abstract_renderer_item.h"
#include "renderer/rendered_items_cache.h"



svg_document::svg_document (wait_queue<abstract_renderer_event> *queue)
{
  m_item_factory = new svg_item_factory (this);
  m_attribute_factory = new svg_attribute_factory (this);
  m_item_container = new svg_items_container;
  m_root = 0;
  item_svg = 0;
  m_queue = queue;
}

svg_document::~svg_document ()
{
  FREE (m_root);
  FREE (m_item_factory);
  FREE (m_attribute_factory);
  FREE (m_item_container);
}

bool svg_document::read_file (const QString &filename_arg)
{
  filename = filename_arg;

  QFile file (filename);
  if (!file.open (QIODevice::ReadOnly))
    return false;

  QDomDocument doc;
  if (!doc.setContent (&file, true))
    return false;

  QDomElement root = doc.documentElement ();
  m_root = m_item_factory->create_item (root.localName (), root.namespaceURI (), root.prefix ());
  m_root->read (root);
  if (m_root->type () == svg_item_type::SVG)
    item_svg = static_cast<svg_item_svg *> (m_root);
  else
    return false;

  return item_svg->check ();
}

QString svg_document::get_filename ()
{
  return filename;
}

bool svg_document::write_file (const QString &filename_arg)
{
  filename = filename_arg;
  QFile file (filename);
  if (!file.open (QIODevice::WriteOnly))
    return false;

  QTextStream stream (&file);

  QDomDocument doc;
  QDomProcessingInstruction instr = doc.createProcessingInstruction ("xml", "version='1.0' encoding='UTF-8'");
  doc.appendChild(instr);

  QDomElement root_item = doc.createElementNS (m_root->namespace_uri (), m_root->name ());
  doc.appendChild (root_item);
  m_root->write (root_item, doc);

  
  stream << doc.toString (1);
  return false;
}

bool svg_document::get_doc_dimensions (double &width, double &height)
{
  if (!item_svg)
    return false;

  width = item_svg->width ();
  height = item_svg->height ();
  return true;
}

renderer_items_container *svg_document::create_rendered_items (rendered_items_cache *cache)
{
  renderer_items_container *renderer_items = new renderer_items_container;
  renderer_items->set_cache (cache);
  if (cache)
    cache->clear_selection_mapping ();

  create_renderer_item (renderer_items, item_svg);
  renderer_items->set_root (item_svg->id ().toStdString ());
  renderer_items->root ()->update_bbox ();
  return renderer_items;
}

void svg_document::create_renderer_item (renderer_items_container *renderer_items, abstract_svg_item *svg_item)
{
  abstract_renderer_item *renderer_item = svg_item->create_renderer_item ();
  if (!renderer_item)
    return;

  renderer_items->add_item (renderer_item);
  if (svg_item->parent ())
    renderer_items->add_child (svg_item->parent ()->id ().toStdString (), svg_item->id ().toStdString ());

  for (abstract_svg_item *child = svg_item->first_child (); child; child = child->next_sibling ())
    create_renderer_item (renderer_items, child);
}
