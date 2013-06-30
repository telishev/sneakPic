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
#include "svg/attributes/svg_attribute_class.h"
#include "svg/attributes/svg_attribute_style.h"
#include "svg_item_style.h"
#include "svg_item_defs.h"


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
  read_item (item.text ());

  QDomNamedNodeMap attributes = item.attributes();
  for (int i = 0; i < attributes.count (); i++)
    {
      QDomNode item = attributes.item (i);
      if (!item.isAttr ())
        continue;

      QDomAttr attr_item = item.toAttr ();
      abstract_attribute *attribute = attribute_factory->create_attribute (this, attr_item.localName (), attr_item.namespaceURI (), attr_item.prefix ());
      attribute->read (attr_item.value ());
      add_attribute (attribute);
    }

  add_to_container ();

  for (QDomNode child = item.firstChild(); !child.isNull(); child = child.nextSibling())
    {
      if (!child.isElement ())
        continue;

      QDomElement child_element = child.toElement ();
      abstract_svg_item *child_item = item_factory->create_item (child_element.localName (), child_element.namespaceURI (), child_element.prefix ());
      child_item->read (child_element);
      insert_child (nullptr, child_item);
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
  m_attributes.insert (std::make_pair (attribute->name (), attribute));
}

void abstract_svg_item::remove_attribute (abstract_attribute *attribute)
{
  m_attributes.erase (attribute->name ());
}

const char *abstract_svg_item::namespace_uri () const 
{
  return svg_namespaces::uri (namespace_type ());
}

const char * abstract_svg_item::namespace_name () const 
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

  m_child_map.insert (std::make_pair (new_child->name (), new_child));
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
  m_child_map.erase (child->name ());
}

bool abstract_svg_item::has_id () const
{
  return get_attribute<svg_attribute_id> () != nullptr;
}

QString abstract_svg_item::id () const
{
  const svg_attribute_id *attribute_id = get_attribute<svg_attribute_id> ();
  if (!attribute_id)
    return QString ();

  return attribute_id->id ();
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

const abstract_attribute *abstract_svg_item::get_computed_attribute (const char *data, bool is_stylable) const
{
  /// 1. search in own attributes
  auto it = m_attributes.find (data);
  if (it != m_attributes.end ())
    return it->second;

  if (!is_stylable)
    return nullptr;

  /// 2. Search in "style" attribute
  const svg_attribute_style *style = get_attribute<svg_attribute_style> ();
  const abstract_attribute *attribute = 0;
  if (style)
    attribute = style->get_attribute (data);
  if (attribute)
    return attribute;

  /// 3. Search in selectors
  attribute = find_attribute_in_selectors (data, this);
  if (attribute)
    return attribute;

  /// 4. Inherit from parent
  if (m_parent)
    {
      attribute = m_parent->get_computed_attribute (data, is_stylable);
      if (attribute)
        return attribute;
    }

  /// 5. If still not found, return nullptr
  return nullptr;
}

bool abstract_svg_item::is_xml_class (const QString &class_name) const
{
  const svg_attribute_class *attribute_class = get_attribute<svg_attribute_class> ();
  if (!attribute_class)
    return false;

  return attribute_class->is_class (class_name);
}

abstract_attribute *abstract_svg_item::get_attribute (const char *data) const
{
  auto it = m_attributes.find (data);
  if (it != m_attributes.end ())
    return it->second;

  return nullptr;
}

const abstract_attribute *abstract_svg_item::find_attribute_in_selectors (const char *data, const abstract_svg_item *item) const
{
  const abstract_attribute *attribute = nullptr;
  /// 1. search in <style> child
  attribute = find_attribute_in_style_item (data, item);
  if (attribute)
    return attribute;

  /// 2. search in parent selectors
  if (!m_parent)
    return nullptr;

  attribute = m_parent->find_attribute_in_selectors (data, item);
  if (attribute)
    return attribute;

  /// 3. search in sibling <defs> items
  auto range = m_child_map.equal_range (svg_item_defs::static_name ());
  for (auto it = range.first; it != range.second; it++)
    {
      const abstract_svg_item *child = it->second;
      attribute = child->find_attribute_in_style_item (data, item);
      if (attribute)
        return attribute;
    }

  /// 4. If not found return nullptr
  return nullptr;
}

const abstract_attribute *abstract_svg_item::find_attribute_in_style_item (const char *data, const abstract_svg_item *item) const
{
  const abstract_attribute *attribute = nullptr;
  auto range = m_child_map.equal_range (svg_item_style::static_name ());
  for (auto it = range.first; it != range.second; it++)
    {
      const svg_item_style *style = static_cast <const svg_item_style *> (it->second);
      attribute = style->get_attribute (data, item);
      if (attribute)
        return attribute;
    }

  return attribute;
}
