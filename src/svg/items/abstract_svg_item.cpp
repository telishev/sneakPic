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
#include "svg/items/svg_item_type.h"
#include "svg/items/svg_character_data.h"

#include "svg/svg_document.h"
#include "svg/svg_namespaces.h"
#include "svg/svg_item_observer.h"
#include "svg/undoable_items_container.h"

#include "svg/css/selectors_container.h"
#include "svg/changed_items_container.h"
#include "svg/undo_handler.h"
#include "svg/simple_item_observer.h"

class svg_item_state : public abstract_state_t
{
public:
  svg_document *m_document;
  std::unordered_map<std::string, int> m_attributes;

  std::string m_original_id;
  std::string m_own_id;

  std::vector<int> m_children;
  int m_parent;

  std::vector<int> m_observers;
  std::vector<int> m_created_observers;

  svg_item_type m_type;
  int m_undo_id;

  virtual undoable * create_new_item () override
  {
    abstract_svg_item *item =  m_document->item_factory ()->create_item (m_type);
    item->set_undo_id (m_undo_id);
    return item;
  }

  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override
  {
    return new simple_state_diff<svg_item_state> (first, second);
  }

  svg_item_state *clone () const
  {
    return new svg_item_state (*this);
  }

};


class cloned_item_observer : public simple_item_observer<cloned_item_observer>
{
public:
  cloned_item_observer (svg_document *document, const std::string &parent)
    : simple_item_observer (document, parent) {}

  virtual void child_added (const std::string &/*parent*/, const std::string &child_name, int insert_pos) override
  {
    abstract_svg_item *child = document ()->item_container ()->get_item (child_name);
    parent ()->insert_child (insert_pos, child->create_clone ());
  }

  virtual void child_removed (const std::string &/*parent*/, const std::string &/*child_name*/, int pos) override
  {
    parent ()->remove_child (parent ()->child (pos));
  }

  virtual void child_moved (const std::string &/*parent*/, const std::string &/*child_name*/, int old_pos, int new_pos) override
  {
    parent ()->move_child (new_pos, parent ()->child (old_pos));
  }

  virtual void attribute_change_start (const std::string &/*parent*/, const abstract_attribute *computed_attribute) override
  {
    const abstract_attribute *attribute = parent ()->get_computed_attribute (computed_attribute->type_name (), computed_attribute->inherit_type (), computed_attribute->type ());
    parent ()->register_item_change (); /// surely not the best way to force item update on undo
    parent ()->signal_attribute_change_start (attribute);
  }

  virtual void attribute_change_end (const std::string &/*parent*/, const abstract_attribute *computed_attribute) override
  {
    const abstract_attribute *attribute = parent ()->get_computed_attribute (computed_attribute->type_name (), computed_attribute->inherit_type (), computed_attribute->type ());
    parent ()->signal_attribute_change_end (attribute);
  }
};

abstract_svg_item::abstract_svg_item (svg_document *document)
{
  m_document = document;
  m_observers = nullptr;
  m_children = nullptr;
  m_created_observers = nullptr;
  m_parent = -1;
  m_document->get_undoable_items_container ()->assign_id (this);
}

abstract_svg_item::~abstract_svg_item ()
{
  remove_from_container ();
  auto container = m_document->get_undoable_items_container ();
  for (auto &attribute : m_attributes)
     container->remove_item (attribute.second);

  if (m_observers)
    {
      auto container = m_document->get_undoable_items_container ();
      for (int id : *m_observers)
        container->remove_item (id);
    }

  if (m_created_observers)
    {
      
      for (int id : *m_created_observers)
        container->remove_item (id);
    }

  FREE (m_observers);
  FREE (m_created_observers);
  FREE (m_children);
}

void abstract_svg_item::write (QXmlStreamWriter &writer) const
{
  if (is_cloned ())
    return;

  if (is_character_data ())
    {
      const svg_character_data *data = static_cast<const svg_character_data *> (this);
      writer.writeCharacters (data->char_data ());
      return;
    }

  writer.writeStartElement (namespace_uri (), type_name ());

  for (auto &attribute_pair : m_attributes)
    {
      abstract_attribute *attribute = get_attribute_by_id (attribute_pair.second);
      QString value;
      if (attribute->is_inherited ())
        value = "inherit";
      else
        attribute->write (value);
      writer.writeAttribute (attribute->namespace_uri (), attribute->type_name (), value);
    }

  for (int i = 0; i < children_count (); i++)
    {
      child (i)->write (writer);
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

  int id = document ()->get_undoable_items_container ()->add_item (attribute);

  m_attributes.insert (std::make_pair (attribute->type_name (), id));
}

void abstract_svg_item::remove_attribute (abstract_attribute *attribute)
{
  if (is_cloned ())
    {
      DEBUG_PAUSE ("remove attribute is not allowed for cloned items");
      return;
    }

  m_attributes.erase (attribute->type_name ());
  document ()->get_undoable_items_container ()->remove_item (attribute->undo_id ());
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
  return !m_own_id.empty ();
}

std::string abstract_svg_item::name () const
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

  for (int i = 0; i < children_count (); i++)
    CHECK (child (i)->check ());

  return true;
}

void abstract_svg_item::add_to_container ()
{
  svg_items_container *container = document ()->item_container ();
  DEBUG_ASSERT (has_name ());
  if (!container->get_item (name ()))
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
  const abstract_attribute *attribute = get_attribute (data, true);

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

bool abstract_svg_item::is_xml_class (const std::string &class_name) const
{
  const svg_attribute_class *attribute_class = get_computed_attribute<svg_attribute_class> ();
  return attribute_class->is_class (class_name);
}

abstract_attribute *abstract_svg_item::get_attribute (const char *data, bool get_clone_attributes) const
{
  /// search in original attributes for cloned items
  if (is_cloned ())
    {
      if (!get_clone_attributes)
        return nullptr;

      const abstract_svg_item *original_item = get_original_item ();
      if (original_item)
        return original_item->get_attribute (data, get_clone_attributes);

      return nullptr;
    }

  auto it = m_attributes.find (data);
  if (it != m_attributes.end ())
    return get_attribute_by_id (it->second);


  return nullptr;
}

const abstract_attribute *abstract_svg_item::find_attribute_in_selectors (const char *data, const abstract_svg_item *item, svg_attribute_type /*attr_type*/) const
{
  return m_document->selectors ()->get_attribute (data, item);
}

bool abstract_svg_item::is_cloned () const
{
  return !m_original_id.empty ();
}

abstract_svg_item *abstract_svg_item::create_clone ()
{
  abstract_svg_item *clone = m_document->item_factory ()->create_item (type_name (), namespace_uri (), namespace_name ());
  /// leave m_attributes empty
  clone->m_original_id = name ();
  clone->m_own_id = m_document->item_container ()->create_unique_name (clone->type_name ());
  clone->add_to_container ();

  /// append cloned children to a clone
  for (int i = 0; i < children_count (); i++)
    {
      abstract_svg_item *cloned_child = child (i)->create_clone ();
      clone->push_back (cloned_child);
    }

  clone->observe_item (this, new cloned_item_observer (m_document, clone->m_own_id));
  return clone;
}

const abstract_svg_item *abstract_svg_item::get_original_item () const
{
  if (m_original_id.empty ())
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
  auto attribute_id = get_attribute_for_change<svg_attribute_id> ();
  attribute_id->set_id (m_own_id);
}

void abstract_svg_item::create_id_by_attr ()
{
  svg_items_container *container = document ()->item_container ();
  if (m_own_id.empty ())
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
      const abstract_attribute *attribute = get_attribute_by_id (attribute_pair.second);
      if (*attribute->namespace_uri ())
        map.insert (std::make_pair (attribute->namespace_uri (), attribute->namespace_name ()));
    }

  for (int i = 0; i < children_count (); i++)
    child (i)->get_used_namespaces (map);
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

bool abstract_svg_item::is_character_data () const
{
  return type () == svg_item_type::CHARACTER_DATA; 
}

int abstract_svg_item::child_index () const
{
  abstract_svg_item *my_parent = parent ();
  if (!my_parent)
    return -1;

  const std::vector<int> *parent_children = my_parent->m_children;
  DEBUG_ASSERT (parent_children);
  auto it = std::find (parent_children->begin (), parent_children->end (), undo_id ());
  DEBUG_ASSERT (it != parent_children->end ());
  return it - parent_children->begin ();
}

int abstract_svg_item::children_count () const
{
  return m_children ? (int)m_children->size () : 0;
}

abstract_svg_item *abstract_svg_item::parent () const
{
  if (m_parent < 0)
    return nullptr;

  return static_cast<abstract_svg_item *> (m_document->get_undoable_items_container ()->get_item (m_parent));
}

abstract_svg_item *abstract_svg_item::child (int index) const
{
  if (!m_children)
    return nullptr;

  return static_cast<abstract_svg_item *> (m_document->get_undoable_items_container ()->get_item ((*m_children)[index]));
}

void abstract_svg_item::push_back (abstract_svg_item *new_child)
{
  return insert_child (children_count (), new_child);
}

void abstract_svg_item::insert_child (int position, abstract_svg_item *new_child)
{
  register_item_change ();
  if (!m_children)
    m_children = new std::vector<int>;
  int id = m_document->get_undoable_items_container ()->add_item (new_child);
  m_children->insert (m_children->begin () + position, id);
  new_child->m_parent = undo_id ();
  signal_child_inserted (new_child->name (), position);
}

void abstract_svg_item::remove_child (abstract_svg_item *child)
{
  register_item_change ();
  DEBUG_ASSERT (m_children);
  signal_child_removed (child->name (), child->child_index ());
  child->signal_item_removed ();
  m_children->erase (std::remove (m_children->begin (), m_children->end (), child->undo_id ()), m_children->end ());
  m_document->get_undoable_items_container ()->remove_item (child->undo_id ());
}

void abstract_svg_item::move_child (int position, abstract_svg_item *child)
{
  register_item_change ();
  int prev_pos = child->child_index ();
  int new_pos = position;
  if (position == prev_pos)
    return;
  if (position > prev_pos)
    new_pos--;

  m_children->erase (std::remove (m_children->begin (), m_children->end (), child->undo_id ()), m_children->end ());
  m_children->insert (m_children->begin () + new_pos, child->undo_id ());

  signal_child_moved (child->name (), prev_pos, position);
}

abstract_svg_item *abstract_svg_item::get_cloned_child (const std::string &child_original) const
{
  /// TODO: make it faster
  if (child_original.empty ())
    return nullptr;

  for (int i = 0; i < children_count (); i++)
    {
      if (child (i)->m_original_id == child_original)
        return child (i);
    }

  return nullptr;
}

abstract_attribute *abstract_svg_item::get_attribute_for_change (const char *data, svg_inherit_type inherit_type, svg_attribute_type attr_type, const abstract_attribute *default_val)
{
  abstract_attribute *attribute = get_attribute (data, false);
  if (attribute && attribute->type () == attr_type)
    {
      attribute->register_change ();
      return attribute;
    }

  const abstract_attribute *computed = get_computed_attribute (data, inherit_type, attr_type);
  if (!computed || computed->type () != attr_type)
    computed = default_val;

  attribute = computed->clone (m_document);
  add_attribute (attribute);
  return attribute;
}

void abstract_svg_item::signal_child_inserted (const std::string &child, int position)
{
  if (!m_document->signals_enabled ())
    return;

  m_document->changed_items ()->child_added (name (), child, position);
  auto container = m_document->get_undoable_items_container ();
  if (m_observers)
    {
      for (int id : *m_observers)
        {
          svg_item_observer *observer = static_cast<svg_item_observer *> (container->get_item (id));
          if (!observer)
            continue;

          observer->child_added (name (), child, position);
        }
    }
}

void abstract_svg_item::signal_child_removed (const std::string &child_name, int pos)
{
  if (!m_document->signals_enabled ())
    return;

  m_document->changed_items ()->child_removed (name (), child_name, pos);
  auto container = m_document->get_undoable_items_container ();
  if (m_observers)
    {
      for (int id : *m_observers)
        {
          svg_item_observer *observer = static_cast<svg_item_observer *> (container->get_item (id));
          if (!observer)
            continue;

          observer->child_removed (name (), child_name, pos);
        }
    }
}

void abstract_svg_item::signal_item_removed ()
{
  if (!m_document->signals_enabled ())
    return;

  m_document->changed_items ()->item_removed (name ());
  auto container = m_document->get_undoable_items_container ();
  if (m_observers)
    {
      for (int id : *m_observers)
        {
          svg_item_observer *observer = static_cast<svg_item_observer *> (container->get_item (id));
          if (!observer)
            continue;

          observer->item_removed (name ());
        }
    }
}

void abstract_svg_item::signal_child_moved (const std::string &child_name, int old_pos, int new_pos)
{
  if (!m_document->signals_enabled ())
    return;

  m_document->changed_items ()->child_moved (name (), child_name, old_pos, new_pos);
  auto container = m_document->get_undoable_items_container ();
  if (m_observers)
    {
      for (int id : *m_observers)
        {
          svg_item_observer *observer = static_cast<svg_item_observer *> (container->get_item (id));
          if (!observer)
            continue;

          observer->child_moved (name (), child_name, old_pos, new_pos);
        }
    }
}

void abstract_svg_item::signal_attribute_change_start (const abstract_attribute *attribute)
{
  if (!m_document->signals_enabled ())
    return;

  m_document->changed_items ()->attribute_change_start (name (), attribute);
  auto container = m_document->get_undoable_items_container ();
  if (m_observers)
    {
      for (int id : *m_observers)
        {
          svg_item_observer *observer = static_cast<svg_item_observer *> (container->get_item (id));
          if (!observer)
            continue;

          observer->attribute_change_start (name (), attribute);
        }
    }
}

void abstract_svg_item::signal_attribute_change_end (const abstract_attribute *attribute)
{
  if (!m_document->signals_enabled ())
    return;

  m_document->changed_items ()->attribute_change_start (name (), attribute);
  auto container = m_document->get_undoable_items_container ();
  if (m_observers)
    {
      for (int id : *m_observers)
        {
          svg_item_observer *observer = static_cast<svg_item_observer *> (container->get_item (id));
          if (!observer)
            continue;

          observer->attribute_change_start (name (), attribute);
        }
    }
}

abstract_state_t *abstract_svg_item::create_state ()
{
  svg_item_state *state = new svg_item_state;
  state->m_document = m_document;
  state->m_attributes = m_attributes;
  state->m_original_id = m_original_id;
  state->m_own_id = m_own_id;
  state->m_undo_id = undo_id ();
  if (m_children)
    state->m_children = *m_children;
  state->m_parent = m_parent;
  state->m_type = type ();
  if (m_observers)
    state->m_observers = *m_observers;
  if (m_created_observers)
    state->m_created_observers = *m_created_observers;

  return state;
}

void abstract_svg_item::load_from_state (const abstract_state_t *abstract_state)
{
  if (!abstract_state)
    {
      document ()->changed_items ()->set_item_removed (name ());
      return;
    }

  const svg_item_state *state = static_cast<const svg_item_state *> (abstract_state);
  m_document = state->m_document;
  m_attributes = state->m_attributes;
  m_original_id = state->m_original_id;
  m_own_id = state->m_own_id;
  m_parent = state->m_parent;
  auto changed_items = document ()->changed_items ();

  if (state->m_children.empty ())
    FREE (m_children);
  else
    {
      if (!m_children)
        m_children = new std::vector<int>;

      *m_children = state->m_children;
    }

  if (state->m_observers.empty ())
    FREE (m_observers);
  else
    {
      if (!m_observers)
        m_observers = new std::vector<int>;
      *m_observers = state->m_observers;
    }

  if (state->m_created_observers.empty ())
    FREE (m_created_observers);
  else
    {
      if (!m_created_observers)
        m_created_observers = new std::vector<int>;
      *m_created_observers = state->m_created_observers;
    }

  add_to_container ();
  changed_items->set_item_changed (name ());
  changed_items->set_item_layout_changed (name ());
}

void abstract_svg_item::observe_item (abstract_svg_item *item_to_observe, svg_item_observer *observer)
{
  if (!m_created_observers)
    m_created_observers = new std::vector<int>;

  item_to_observe->add_observer (observer);
  m_created_observers->push_back (observer->undo_id ());
}

void abstract_svg_item::add_observer (svg_item_observer *observer)
{
  if (!m_observers)
    m_observers = new std::vector<int>;

  int id = m_document->get_undoable_items_container ()->add_item (observer);
  m_observers->push_back (id);
}

void abstract_svg_item::register_item_change ()
{
  auto handler = document ()->get_undo_handler ();
  handler->register_item (this);
}

void abstract_svg_item::erase_created_observer (svg_item_observer *observer)
{
  if (!m_observers)
    return;

  m_observers->erase (std::find (m_observers->begin (), m_observers->end (), observer->undo_id ()), m_observers->end ());
}

abstract_attribute *abstract_svg_item::get_attribute_by_id (int id) const
{
  return static_cast<abstract_attribute *> (m_document->get_undoable_items_container ()->get_item (id));
}

abstract_svg_item *abstract_svg_item::get_item_by_id (int id) const
{
  return static_cast<abstract_svg_item *> (m_document->get_undoable_items_container ()->get_item (id));
}

