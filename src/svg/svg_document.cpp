#include "svg/svg_document.h"

#include "svg/items/svg_item_factory.h"
#include "svg/items/svg_named_items_container.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/svg_namespaces.h"

#include "svg/attributes/svg_attribute_factory.h"

#include "svg/items/abstract_svg_item.h"

#include <QFile>
#include <QDomDocument>
#include <QTextStream>


svg_document::svg_document ()
{
  m_item_factory = new svg_item_factory (this);
  m_attribute_factory = new svg_attribute_factory (this);
  m_item_container = new svg_named_items_container;
  m_root = 0;
}

svg_document::~svg_document ()
{
  delete m_item_factory;
  delete m_attribute_factory;
  delete m_item_container;
  delete m_root;
}

bool svg_document::read_file (const QString &filename)
{
  QFile file (filename);
  if (!file.open (QIODevice::ReadOnly))
    return false;

  QDomDocument doc;
  if (!doc.setContent (&file, true))
    return false;

  QDomElement root = doc.documentElement ();
  m_root = m_item_factory->create_item (root.localName (), root.namespaceURI (), root.prefix ());
  m_root->read (root);
  return true;
}

bool svg_document::write_file (const QString &filename)
{
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