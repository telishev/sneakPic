#include "svg/svg_reader.h"

#include <QFile>
#include <QXmlStreamReader>

#include <unordered_set>

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_character_data.h"
#include "svg/items/svg_item_factory.h"

#include "svg/attributes/svg_attribute_factory.h"
#include "svg/attributes/svg_attribute_type.h"

#include "svg/undo/undo_handler.h"
#include "attributes/svg_attribute_style.h"
#include "items/svg_item_style.h"
#include "css/selectors_container.h"
#include "attributes/svg_attribute_element_mapping.h"

svg_reader::svg_reader (undo_handler *handler, svg_item_factory *item_factory, svg_document *document)
{
  m_item_factory = item_factory;
  m_undo_handler = handler;
  m_document = document;
  m_root = nullptr;

  m_selectors = new selectors_container;
}

svg_reader::~svg_reader ()
{
  FREE (m_selectors);
}

bool svg_reader::read_file (const QString &filename)
{
  QFile file (filename);
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
                  svg_character_data *data = new svg_character_data (m_document, reader.text ().toUtf8 ().constData ());
                  cur_item->push_back (data);
                }
              break;
            }
          case QXmlStreamReader::EndElement:
            {
              if (!cur_item)
                DEBUG_PAUSE ("cur_item must not be nullptr");

              if (cur_item->type () == svg_item_type::STYLE)
                {
                  svg_item_style *style = static_cast<svg_item_style *>(cur_item);
                  style->add_style_to_container (m_selectors);
                  abstract_svg_item *parent = cur_item->parent ();
                  parent->remove_child (cur_item);
                  cur_item = parent;
                }
              else
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

  if (!m_selectors->empty ())
    process_selectors (m_root);
  return true;
}

static inline QString get_namespace_name (const QXmlStreamNamespaceDeclarations &declarations, const QString &uri)
{
  for (int i = 0; i < declarations.size (); i++)
    if (declarations[i].namespaceUri () == uri)
      return declarations[i].prefix ().toString ();

  return QString ();
}

abstract_svg_item *svg_reader::process_new_item (QXmlStreamReader &reader, abstract_svg_item *cur_item)
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
  svg_attribute_style *style = nullptr;

  for (int i = 0; i < attributes.size (); i++)
    {
      QStringRef attribute_namespace_uri = attributes[i].namespaceUri ();
      QStringRef attribute_name = attributes[i].name ();
      abstract_attribute *attribute = svg_attribute_factory::get ()->create_attribute (child_item,
        attribute_name.toLatin1 ().constData (), attribute_namespace_uri.toLatin1 ().constData ());

      if (!attribute)
        continue;

      if (attribute->inherit_type () != svg_inherit_type::NONE && attributes[i].value () == "inherit")
        attribute->set_is_inherited (true);

      if (!attribute->read (attributes[i].value ().toLatin1 ().constData ()))
        {
          FREE (attribute);
          continue;
        }

      if (attribute->type () == svg_attribute_type::STYLE)
        {
          style = static_cast<svg_attribute_style *> (attribute);
          continue;
        }

      child_item->add_attribute (attribute);
    }

  if (style)
    {
      add_style (child_item, style);
      FREE (style);
    }

  
  child_item->process_after_read ();
  return child_item;
}

void svg_reader::add_style (abstract_svg_item *child_item, svg_attribute_style *style)
{
  const std::map<std::string, abstract_attribute *> &attributes = style->attributes ();
  for (auto attr_pair : attributes)
    {
      abstract_attribute *attr = attr_pair.second;
      if (child_item->has_attribute (attr->type_name ()))
        continue;

      child_item->add_attribute (attr->clone ());
    }
}

void svg_reader::process_selectors (abstract_svg_item *root)
{
  if (!root)
    return;
  const svg_attribute_element_mapping *mapping = svg_attribute_element_mapping::get ();
  std::unordered_map<std::string, abstract_attribute *> attributes;
  m_selectors->get_attributes_for_item (root, attributes);
  for (auto attribute_pair : attributes)
    {
      abstract_attribute *attribute = attribute_pair.second;
      if (root->has_attribute (attribute_pair.first))
        continue;

      if (!mapping->can_be_specified (root->type (), attribute->type ()))
        continue;

      root->add_attribute (attribute->clone ());
    }

  for (int i = 0; i < root->children_count (); i++)
    process_selectors (root->child (i));
}


