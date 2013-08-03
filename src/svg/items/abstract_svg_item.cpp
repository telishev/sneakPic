#include "svg/items/abstract_svg_item.h"

#include <QXmlStreamWriter>

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"
#include "common/common_utils.h"

#include "svg/attributes/svg_attribute_id.h"
#include "svg/attributes/svg_attribute_factory.h"
#include "svg/attributes/svg_attribute_class.h"
#include "svg/attributes/svg_attribute_style.h"
#include "svg/attributes/svg_attribute_type.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attribute_element_mapping.h"

#include "svg/items/svg_item_factory.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/svg_item_style.h"
#include "svg/items/svg_item_defs.h"

#include "svg/svg_document.h"
#include "svg/svg_namespaces.h"

#include "svg/css/selectors_container.h"


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

void abstract_svg_item::write (QXmlStreamWriter &writer) const
{
  if (is_cloned ())
    return;

  writer.writeStartElement (namespace_uri (), type_name ());
  QString item_data;
  write_item (item_data);
  if (!item_data.isEmpty ())
    writer.writeCharacters (item_data);

  for (auto &attribute_pair : m_attributes)
    {
      abstract_attribute *attribute = attribute_pair.second;
      QString value;
      if (attribute->is_inherited ())
        value = "inherit";
      else
        attribute->write (value);
      writer.writeAttribute (attribute->namespace_uri (), attribute->type_name (), value);
    }

  for (const abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    {
      child->write (writer);
    }

  writer.writeEndElement ();
}

void abstract_svg_item::add_attribute (abstract_attribute *attribute)
{
  if (is_cloned ())
    {
      DEBUG_PAUSE ("One does not simply add attributes to cloned items");
      return;
    }
  m_attributes.insert (std::make_pair (attribute->type_name (), attribute));
}

void abstract_svg_item::remove_attribute (abstract_attribute *attribute)
{
  if (is_cloned ())
    {
      DEBUG_PAUSE ("remove attribute is not allowed for cloned items");
      return;
    }
  m_attributes.erase (attribute->type_name ());
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

bool abstract_svg_item::has_name () const
{
  return !m_own_id.isEmpty ();
}

QString abstract_svg_item::name () const
{
  return m_own_id;
}

bool abstract_svg_item::check ()
{
  if (!check_item ())
    return false;

  /// for items that don't have a name, generate it
  if (!has_name ())
    {
      create_unique_name ();
      add_to_container ();
    }

  for (abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    CHECK (child->check ());

  return true;
}

void abstract_svg_item::add_to_container ()
{
  svg_items_container *container = document ()->item_container ();
  DEBUG_ASSERT (has_name ());
  container->add_item (this);
}

void abstract_svg_item::remove_from_container ()
{
  if (has_name ())
    document ()->item_container ()->remove_item (this);
}

const abstract_attribute *abstract_svg_item::get_computed_attribute (const char *data, svg_inherit_type inherit_type, svg_attribute_type attr_type) const
{
  /// 1. search in own attributes
  const svg_attribute_element_mapping *mapping = svg_attribute_element_mapping::get ();
  bool can_be_specified = mapping->can_be_specified (type (), attr_type);
  const abstract_attribute *attribute = get_attribute (data);

  if (attribute && attribute->is_inherited ())
    return parent () ? parent ()->get_computed_attribute (data, inherit_type, attr_type) : nullptr;

  if (attribute && attribute->type () == attr_type)
    return attribute;

  if (inherit_type == svg_inherit_type::NONE)
    return nullptr;

  if (inherit_type == svg_inherit_type::STYLE && can_be_specified)
    {
      /// 2. Search in "style" attribute
      const svg_attribute_style *style = get_computed_attribute<svg_attribute_style> ();
      if (style)
        attribute = style->get_attribute (data);
      if (attribute && attribute->type () == attr_type)
        return attribute;

      /// 3. Search in selectors
      /// css selectors for cloned items works as they were applied to an original item
      if (is_cloned ())
        {
          const abstract_svg_item *original_item = get_original_item ();
          if (original_item)
            attribute = original_item->find_attribute_in_selectors (data, original_item, attr_type);
        }
      else
        attribute = find_attribute_in_selectors (data, this, attr_type);
      if (attribute)
        return attribute;
    }

  /// 4. Inherit from parent
  if (parent ())
    {
      attribute = parent ()->get_computed_attribute (data, inherit_type, attr_type);
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

const abstract_attribute *abstract_svg_item::find_attribute_in_selectors (const char *data, const abstract_svg_item *item, svg_attribute_type /*attr_type*/) const
{
  return m_document->selectors ()->get_attribute (data, item);
}

bool abstract_svg_item::is_cloned () const
{
  return !m_original_id.isEmpty ();
}

abstract_svg_item *abstract_svg_item::create_clone ()
{
  abstract_svg_item *clone = m_document->item_factory ()->create_item (type_name (), namespace_uri (), namespace_name ());
  /// leave m_attributes empty
  clone->m_original_id = name ();
  clone->m_own_id = m_document->item_container ()->create_unique_name (clone->type_name ());
  clone->add_to_container ();

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

void abstract_svg_item::create_unique_name ()
{
  if (is_cloned ())
    {
      DEBUG_PAUSE ("cloned items shouldn't be here");
      return;
    }

  svg_items_container *container = document ()->item_container ();
  m_own_id = container->create_unique_name (type_name ());
  svg_attribute_id *attribute_id = get_attribute<svg_attribute_id> ();
  if (!attribute_id)
    {
      attribute_id = new svg_attribute_id (this);
      add_attribute (attribute_id);
    }

  attribute_id->set_id (m_own_id);
}

void abstract_svg_item::create_id_by_attr ()
{
  svg_items_container *container = document ()->item_container ();
  if (m_own_id.isEmpty ())
    m_own_id = get_computed_attribute<svg_attribute_id> ()->id ();

  if (container->contains (name ()))
    create_unique_name ();
}

void abstract_svg_item::get_used_namespaces (std::map<QString, QString> &map) const
{
  if (*namespace_uri ())
    map.insert (std::make_pair (namespace_uri (), namespace_name ()));

  for (auto &attribute_pair : m_attributes)
    {
      const abstract_attribute *attribute = attribute_pair.second;
      if (*attribute->namespace_uri ())
        map.insert (std::make_pair (attribute->namespace_uri (), attribute->namespace_name ()));
    }

  for (const abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    child->get_used_namespaces (map);
}

void abstract_svg_item::process_after_read ()
{
  /// if item has a name, add it to container before children
  /// we cannot generate unique name right now for items that don't have it
  /// because that name could be used by next items in file
  create_id_by_attr ();
  if (has_name ())
    add_to_container ();
}


