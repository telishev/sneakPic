#include "svg/items/abstract_svg_item.h"

#include <QDomElement>

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"
#include "common/common_utils.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_id.h"
#include "svg/attributes/svg_attribute_factory.h"
#include "svg/attributes/svg_attribute_class.h"
#include "svg/attributes/svg_attribute_style.h"
#include "svg/attributes/attribute_type.h"

#include "svg/items/svg_item_factory.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/svg_item_style.h"
#include "svg/items/svg_item_defs.h"

#include "svg/svg_document.h"
#include "svg/svg_namespaces.h"




abstract_svg_item::abstract_svg_item (svg_document *document)
{
  m_document = document;
}

abstract_svg_item::~abstract_svg_item ()
{
  remove_from_container ();
  for (auto &attribute : m_attributes)
    delete attribute.second;
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
      if (attribute->read (attr_item.value ()))
        add_attribute (attribute);
      else
        FREE (attribute);
    }

  /// if item has a name, add it to container before children
  /// we cannot generate unique name right now for items that don't have it
  /// because that name could be used by next items in file
  add_to_container (false);

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
  if (is_cloned ())
    return;

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
  if (is_cloned ())
    {
      DEBUG_PAUSE ("One does not simply add attributes to cloned items");
      return;
    }
  m_attributes.insert (std::make_pair (attribute->name (), attribute));
}

void abstract_svg_item::remove_attribute (abstract_attribute *attribute)
{
  if (is_cloned ())
    {
      DEBUG_PAUSE ("remove attribute is not allowed for cloned items");
      return;
    }
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

bool abstract_svg_item::has_id () const
{
  return !m_own_id.isEmpty ();
}

QString abstract_svg_item::id () const
{
  return m_own_id;
}

bool abstract_svg_item::check ()
{
  if (!check_item ())
    return false;

  /// for items that don't have a name, generate it
  add_to_container (true);

  for (abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    CHECK (child->check ());

  return true;
}

void abstract_svg_item::add_to_container (bool force_name)
{
  svg_items_container *container = document ()->item_container ();
  update_own_id (force_name);

  if (has_id ())
    container->add_item (this);
}

void abstract_svg_item::remove_from_container ()
{
  if (has_id ())
    document ()->item_container ()->remove_item (this);
}

const abstract_attribute *abstract_svg_item::get_computed_attribute (const char *data, svg_inherit_type inherit_type) const
{
  /// 1. search in own attributes
  const abstract_attribute *attribute = get_attribute (data);
  if (attribute)
    return attribute;

  if (inherit_type == svg_inherit_type::NONE)
    return nullptr;

  if (inherit_type == svg_inherit_type::STYLE)
    {
      /// 2. Search in "style" attribute
      const svg_attribute_style *style = get_computed_attribute<svg_attribute_style> ();
      if (style)
        attribute = style->get_attribute (data);
      if (attribute)
        return attribute;

      /// 3. Search in selectors
      /// css selectors for cloned items works as they were applied to an original item
      if (is_cloned ())
        {
          const abstract_svg_item *original_item = get_original_item ();
          if (original_item)
            attribute = original_item->find_attribute_in_selectors (data, original_item);
        }
      else
        attribute = find_attribute_in_selectors (data, this);
      if (attribute)
        return attribute;
    }

  /// 4. Inherit from parent
  if (parent ())
    {
      attribute = parent ()->get_computed_attribute (data, inherit_type);
      if (attribute)
        return attribute;
    }

  /// 5. If still not found, return nullptr
  return nullptr;
}

bool abstract_svg_item::is_xml_class (const QString &class_name) const
{
  const svg_attribute_class *attribute_class = get_computed_attribute<svg_attribute_class> ();
  return attribute_class->is_class (class_name);
}

abstract_attribute *abstract_svg_item::get_attribute (const char *data) const
{
  /// search in original attributes for cloned items
  if (is_cloned ())
    {
      const abstract_svg_item *original_item = get_original_item ();
      if (original_item)
        return original_item->get_attribute (data);

      return nullptr;
    }

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
  if (!parent ())
    return nullptr;

  attribute = parent ()->find_attribute_in_selectors (data, item);
  if (attribute)
    return attribute;

  /// 3. search in sibling <defs> items
  auto range = get_childs_by_name (svg_item_defs::static_name ());
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
  auto range = get_childs_by_name (svg_item_style::static_name ());
  for (auto it = range.first; it != range.second; it++)
    {
      const svg_item_style *style = static_cast <const svg_item_style *> (it->second);
      attribute = style->get_attribute (data, item);
      if (attribute)
        return attribute;
    }

  return attribute;
}

bool abstract_svg_item::is_cloned () const
{
  return !m_original_id.isEmpty ();
}

abstract_svg_item *abstract_svg_item::create_clone ()
{
  abstract_svg_item *clone = m_document->item_factory ()->create_item (name (), namespace_uri (), namespace_name ());
  /// leave m_attributes empty
  clone->m_original_id = id ();
  clone->m_own_id = m_document->item_container ()->create_unique_name (clone->name ());
  
  /// append cloned children to a clone
  for (abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    {
      abstract_svg_item *cloned_child = child->create_clone ();
      clone->insert_child (nullptr, cloned_child);
    }

  return clone;
}

const abstract_svg_item *abstract_svg_item::get_original_item () const
{
  if (m_original_id.isEmpty ())
    return nullptr;

  return m_document->item_container ()->get_item (m_original_id);
}

void abstract_svg_item::update_own_id (bool force_name)
{
  svg_items_container *container = document ()->item_container ();
  if (m_own_id.isEmpty ())
    m_own_id = get_computed_attribute<svg_attribute_id> ()->id ();

  if (!force_name)
    return;

  if (!has_id () || container->contains (id ()))
    {
      if (is_cloned ())
        {
          DEBUG_PAUSE ("cloned items shouldn't be here");
          return;
        }

      m_own_id = container->create_unique_name (name ());
      svg_attribute_id *attribute_id = get_attribute<svg_attribute_id> ();
      if (!attribute_id)
        {
          attribute_id = new svg_attribute_id (this);
          add_attribute (attribute_id);
        }

      attribute_id->set_id (m_own_id);
    }
}

