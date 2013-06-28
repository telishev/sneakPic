#include "svg/items/abstract_svg_item.h"

#include <QDomElement>

#include "common/debug_utils.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_id.h"
#include "svg/attributes/svg_attribute_factory.h"

#include "svg/items/svg_item_factory.h"
#include "svg/items/svg_items_container.h"

#include "svg/svg_document.h"
#include "svg/svg_namespaces.h"
#include "../attributes/svg_attribute_class.h"


abstract_svg_item::abstract_svg_item (svg_document *document)
{
  m_document = document;
  m_parent = nullptr;
  m_next_sibling = nullptr;
  m_prev_sibling = nullptr;

  m_first_child = nullptr;
  m_last_child = nullptr;
  m_id = nullptr;
  m_class = nullptr;
}

abstract_svg_item::~abstract_svg_item ()
{
  remove_from_container ();
  for (auto &attribute : m_attributes)
    delete attribute.second;

  abstract_svg_item *cur = m_first_child, *next = nullptr;
  while (cur)
    {
      next = cur->next_sibling ();
      delete cur;
      cur = next;
    }
}

void abstract_svg_item::read (const QDomElement &item)
{
  svg_item_factory *item_factory = m_document->item_factory ();
  svg_attribute_factory *attribute_factory = m_document->attribute_factory ();

  for (QDomNode child = item.firstChild(); !child.isNull(); child = child.nextSibling())
    {
      if (!child.isElement ())
        continue;

      QDomElement child_element = child.toElement ();
      abstract_svg_item *child_item = item_factory->create_item (child_element.localName (), child_element.namespaceURI (), child_element.prefix ());
      child_item->read (child_element);
      insert_child (nullptr, child_item);
    }

  QDomNamedNodeMap attributes = item.attributes();
  for (int i = 0; i < attributes.count (); i++)\
    {
      QDomNode item = attributes.item (i);
      if (!item.isAttr ())
        continue;

      QDomAttr attr_item = item.toAttr ();
      abstract_attribute *attribute = attribute_factory->create_attribute (attr_item.localName (), attr_item.namespaceURI (), attr_item.prefix ());
      attribute->read (attr_item.value ());
      add_attribute (attribute);
      process_attribute (attribute);
    }

  
}

void abstract_svg_item::write (QDomElement &item, QDomDocument &doc) const 
{
  for (const abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    {
      QDomElement child_element = doc.createElementNS (child->namespace_uri (), full_name (child->namespace_name (), child->name ()));
      child->write (child_element, doc);
      item.appendChild (child_element);
    }

  for (auto &attribute_pair : m_attributes)
    {
      abstract_attribute *attribute = attribute_pair.second;
      QDomAttr dom_attribute = doc.createAttributeNS (attribute->namespace_uri (), full_name (attribute->namespace_name (), attribute->name ()));

      QString value;
      attribute->write (value);
      dom_attribute.setValue (value);
      item.setAttributeNode (dom_attribute);
    }
}

void abstract_svg_item::add_attribute (abstract_attribute *attribute)
{
  m_attributes.insert (std::make_pair (attribute->name ().toStdString (), attribute));
}

void abstract_svg_item::remove_attribute (abstract_attribute *attribute)
{
  m_attributes.erase (attribute->name ().toStdString ());
}

QString abstract_svg_item::namespace_uri () const 
{
  return svg_namespaces::uri (namespace_type ());
}

QString abstract_svg_item::namespace_name () const 
{
  return svg_namespaces::name (namespace_type ());
}

QString abstract_svg_item::full_name (const QString &namespace_name, const QString &local_name) const
{
  if (namespace_name.isEmpty ())
    return local_name;

  return namespace_name + ":" + local_name;
}

void abstract_svg_item::insert_child (abstract_svg_item *position, abstract_svg_item *new_child)
{
  DEBUG_ASSERT (new_child && new_child->parent () == nullptr);
  abstract_svg_item *prev = position ? position->prev_sibling () : m_last_child;
  abstract_svg_item *next = position;

  new_child->set_next_sibling (next);
  new_child->set_prev_sibling (prev);
  new_child->set_parent (this);

  if (prev)
    prev->set_next_sibling (new_child);

  if (next)
    next->set_prev_sibling (new_child);

  if (next == m_first_child)
    m_first_child = new_child;

  if (prev == m_last_child)
    m_last_child = new_child;
}

void abstract_svg_item::remove_child (abstract_svg_item *child)
{
  DEBUG_ASSERT (child && child->parent () == this);
  abstract_svg_item *prev = child->prev_sibling ();
  abstract_svg_item *next = child->next_sibling ();

  if (prev)
    prev->set_next_sibling (next);

  if (next)
    next->set_prev_sibling (prev);

  if (child == m_first_child)
    m_first_child = next;

  if (child == m_last_child)
    m_last_child = prev;

  child->set_parent (nullptr);
  child->set_next_sibling (nullptr);
  child->set_prev_sibling (nullptr);
}

void abstract_svg_item::process_attribute (abstract_attribute *attribute)
{
  switch (attribute->type ())
    {
    case svg_attribute_type::ID:
      m_id = static_cast<svg_attribute_id *> (attribute);
      add_to_container ();
      break;
    case svg_attribute_type::CLASS:
      m_class = static_cast<svg_attribute_class *> (attribute);
      break;
    default:
      break;
    }

}

QString abstract_svg_item::id () const
{
  if (!has_id ())
    return QString ();

  return m_id->id ();
}

void abstract_svg_item::set_id (const QString &id)
{
  remove_from_container ();
  if (!m_id)
    {
      m_id = new svg_attribute_id;
      add_attribute (m_id);
    }

  m_id->set_id (id);
  add_to_container ();
}

void abstract_svg_item::add_to_container ()
{
  if (has_id ())
    document ()->item_container ()->add_item (this);
}

void abstract_svg_item::remove_from_container ()
{
  if (has_id ())
    document ()->item_container ()->remove_item (this);
}

abstract_attribute *abstract_svg_item::get_attribute (const QString &data) const
{
  auto it = m_attributes.find (data.toStdString ());
  if (it == m_attributes.end ())
    return nullptr;

  return it->second;
}

bool abstract_svg_item::is_xml_class (const QString &class_name) const
{
  if (!m_class)
    return false;

  return m_class->is_class (class_name);
}
