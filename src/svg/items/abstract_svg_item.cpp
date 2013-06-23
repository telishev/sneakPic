#include "svg/items/abstract_svg_item.h"

#include <QDomElement>

#include "svg/attributes/abstract_attribute.h"
#include "svg/items/svg_item_factory.h"
#include "svg/attributes/svg_attribute_factory.h"
#include "svg/svg_document.h"
#include "svg/svg_namespaces.h"

#include "common/debug_utils.h"


abstract_svg_item::abstract_svg_item (svg_document *document)
{
  m_document = document;
  m_parent = nullptr;
  m_next_sibling = nullptr;
  m_prev_sibling = nullptr;

  m_first_child = nullptr;
  m_last_child = nullptr;
}

abstract_svg_item::~abstract_svg_item ()
{
  for (abstract_attribute *attribute : m_attributes)
    delete attribute;

  abstract_svg_item *cur = m_first_child, *next = 0;
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
      insert_child (0, child_item);
    }

  QDomNamedNodeMap attributes = item.attributes();
  for (int i = 0; i < attributes.count (); i++)
    {
      QDomNode item = attributes.item (i);
      if (!item.isAttr ())
        continue;

      QDomAttr attr_item = item.toAttr ();
      abstract_attribute *attribute = attribute_factory->create_attribute (attr_item.localName (), attr_item.namespaceURI (), attr_item.prefix ());
      attribute->read (attr_item);
      add_attribute (attribute);
    }

  post_read ();
}

void abstract_svg_item::write (QDomElement &item, QDomDocument &doc) const 
{
  for (const abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    {
      QDomElement child_element = doc.createElementNS (child->namespace_uri (), full_name (child->namespace_name (), child->name ()));
      child->write (child_element, doc);
      item.appendChild (child_element);
    }

  for (const abstract_attribute *attribute : m_attributes)
    {
      QDomAttr dom_attribute = doc.createAttributeNS (attribute->namespace_uri (), full_name (attribute->namespace_name (), attribute->name ()));

      attribute->write (dom_attribute);
      item.setAttributeNode (dom_attribute);
    }
}

void abstract_svg_item::add_attribute (abstract_attribute *attribute)
{
  m_attributes.insert (attribute);
}

void abstract_svg_item::remove_attribute (abstract_attribute *attribute)
{
  m_attributes.erase (attribute);
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
