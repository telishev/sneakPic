#include "svg/items/abstract_svg_item.h"

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"
#include "common/common_utils.h"

#include "svg/attributes/svg_attribute_id.h"
#include "svg/attributes/svg_attribute_type.h"

#include "svg/items/svg_item_factory.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/svg_item_type.h"

#include "svg/svg_document.h"
#include "svg/svg_namespaces.h"
#include "svg/svg_item_observer.h"

#include "svg/changed_items_container.h"
#include "svg/undo/undo_handler.h"
#include "svg/simple_item_observer.h"
#include "svg/attributes/svg_attribute_factory.h"
#include "common/range_algorithm.h"

class svg_item_state : public abstract_state_t
{
public:
  svg_document *m_document;
  map<string, int> m_attributes;

  string m_original_id;
  string m_own_id;

  vector<int> m_children;
  int m_parent;

  vector<int> m_observers;
  vector<int> m_created_observers;

  svg_item_type m_type;

  virtual undoable *create_new_item () override
  {
    abstract_svg_item *item =  m_document->item_factory ()->create_item (m_type);
    return item;
  }

  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override
  {
    auto first_state = static_cast<const svg_item_state *> (first);
    auto second_state = static_cast<const svg_item_state *> (second);
    bool recreate = false;
    if (first_state && second)
      recreate = first_state->m_type != second_state->m_type;

    return new simple_state_diff<svg_item_state> (first, second, recreate);
  }

  svg_item_state *clone () const
  {
    return new svg_item_state (*this);
  }

};


class cloned_item_observer : public simple_item_observer<cloned_item_observer>
{
public:
  cloned_item_observer (svg_items_container *container, const string &parent)
    : simple_item_observer (container, parent) {}

  virtual void child_added (const string &/*parent*/, const string &child_name, int insert_pos) override
  {
    abstract_svg_item *child = container ()->get_item (child_name);
    parent ()->insert_child (insert_pos, child->create_clone ());
  }

  virtual void child_removed (const string &/*parent*/, const string &/*child_name*/, int pos) override
  {
    parent ()->remove_child (parent ()->child (pos));
  }

  virtual void child_moved (const string &/*parent*/, const string &/*child_name*/, int old_pos, int new_pos) override
  {
    parent ()->move_child (new_pos, parent ()->child (old_pos));
  }

  virtual void attribute_change_start (const string &/*parent*/, const abstract_attribute *computed_attribute) override
  {
    const abstract_attribute *attribute = parent ()->get_computed_attribute (computed_attribute->type_name (), computed_attribute->inherit_type (), computed_attribute->type ());
    if (!attribute)
      attribute = computed_attribute->default_value ();
    parent ()->register_item_change (); /// surely not the best way to force item update on undo
    parent ()->signal_attribute_change_start (attribute);
  }

  virtual void attribute_change_end (const string &/*parent*/, const abstract_attribute *computed_attribute) override
  {
    const abstract_attribute *attribute = parent ()->get_computed_attribute (computed_attribute->type_name (), computed_attribute->inherit_type (), computed_attribute->type ());
    if (!attribute)
      attribute = computed_attribute->default_value ();
    parent ()->signal_attribute_change_end (attribute);
  }

  virtual void item_replaced (const string &sender_name) override
  {
    abstract_svg_item *sender = container ()->get_item (sender_name);
    abstract_svg_item *clone = sender->document ()->item_factory ()->create_item (sender->type_name (), sender->namespace_uri (), sender->namespace_name ());
    clone->replace_item (parent (), true);
  }
};

abstract_svg_item::abstract_svg_item (svg_document *document)
{
  m_document = document;
  m_observers = nullptr;
  m_created_observers = nullptr;
  m_parent = -1;
  m_document->get_undo_handler ()->assign_id (this);
}

abstract_svg_item::~abstract_svg_item ()
{
  FREE (m_observers);
  FREE (m_created_observers);
}

void abstract_svg_item::add_attribute (abstract_attribute *attribute)
{
  if (is_cloned ())
    {
      DEBUG_PAUSE ("One does not simply add attributes to cloned items");
      return;
    }

  register_item_change ();
  int id = document ()->get_undo_handler ()->add_item (attribute);
  attribute->set_edit_handler (m_document->items_edit_handler ());
  attribute->set_item (undo_id ());

  m_attributes.insert (std::make_pair (attribute->type_name (), id));
}

void abstract_svg_item::remove_attribute (const abstract_attribute *attribute)
{
  if (is_cloned ())
    {
      DEBUG_PAUSE ("remove attribute is not allowed for cloned items");
      return;
    }

  register_item_change ();
  m_attributes.erase (attribute->type_name ());
  document ()->get_undo_handler ()->remove_item (attribute->undo_id ());
}

const char *abstract_svg_item::namespace_uri () const
{
  return svg_namespaces::uri (namespace_type ());
}

const char *abstract_svg_item::namespace_name () const
{
  return svg_namespaces::name (namespace_type ());
}

bool abstract_svg_item::has_name () const
{
  return !m_own_id.empty ();
}

string abstract_svg_item::name () const
{
  return m_own_id;
}

bool abstract_svg_item::process_after_read ()
{
  if (!is_cloned () && !process_item_after_read ())
    return false;

  /// for items that don't have a name, generate it
  update_name();

  for (int i = 0; i < children_count (); i++)
    CHECK (child (i)->process_after_read ());

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
  const abstract_svg_item *cur_item = is_cloned () ? get_original_item () : this;
  if (!cur_item)
    return nullptr;

  /// 1. search in own attributes
  const abstract_attribute *attribute = cur_item->get_attribute (data, true);

  if (attribute && attribute->is_inherited ())
    return cur_item->parent () ? cur_item->parent ()->get_computed_attribute (data, inherit_type, attr_type) : nullptr;

  if (attribute && attribute->type () == attr_type)
    return attribute;

  if (inherit_type == svg_inherit_type::NONE)
    return nullptr;

  /// 2. Inherit from parent
  if (parent ())
    {
      attribute = parent ()->get_computed_attribute (data, inherit_type, attr_type);
      if (attribute)
        return attribute;
    }

  /// 3. If still not found, return nullptr
  return nullptr;
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

  clone->observe_item (this, new cloned_item_observer (m_document->item_container (), clone->m_own_id));
  return clone;
}

abstract_svg_item *abstract_svg_item::get_original_item () const
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

void abstract_svg_item::register_item_name ()
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

  const vector<int> &parent_children = my_parent->m_children;
  auto it = std::find (parent_children.begin (), parent_children.end (), undo_id ());
  DEBUG_ASSERT (it != parent_children.end ());
  return it - parent_children.begin ();
}

abstract_svg_item *abstract_svg_item::sibling (int index) const
{
  abstract_svg_item *cur_parent = parent ();
  return cur_parent ? cur_parent->child (index) : nullptr;
}

int abstract_svg_item::children_count () const
{
  return (int) m_children.size ();
}

abstract_svg_item *abstract_svg_item::parent () const
{
  if (m_parent < 0)
    return nullptr;

  return static_cast<abstract_svg_item *> (m_document->get_undo_handler ()->get_item (m_parent));
}

abstract_svg_item *abstract_svg_item::child (int index) const
{
  return static_cast<abstract_svg_item *> (m_document->get_undo_handler ()->get_item (m_children [index]));
}

void abstract_svg_item::push_back (abstract_svg_item *new_child)
{
  return insert_child (children_count (), new_child);
}

void abstract_svg_item::insert_child (int position, abstract_svg_item *new_child)
{
  register_item_change ();

  int id = new_child->undo_id ();
  if (id < 0 || !m_document->get_undo_handler ()->get_item (id))
    id = m_document->get_undo_handler ()->add_item (new_child);

  m_children.insert (m_children.begin () + position, id);
  new_child->m_parent = undo_id ();
  signal_child_inserted (new_child->name (), position);
}

void abstract_svg_item::remove_child (abstract_svg_item *child)
{
  register_item_change ();
  signal_child_removed (child->name (), child->child_index ());
  child->signal_item_removed ();
  range::erase (m_children, child->undo_id ());
  child->prepare_to_remove ();
  m_document->get_undo_handler ()->remove_item (child->undo_id ());
}

void abstract_svg_item::remove_children ()
{
  auto child_copy = m_children;
  for (int child : child_copy)
    remove_child (static_cast<abstract_svg_item *> (m_document->get_undo_handler ()->get_item (child)));
}


void abstract_svg_item::make_orphan (abstract_svg_item *child)
{
  register_item_change ();
  child->register_item_change ();
  signal_layout_changed ();
  child->m_parent = 0;
  range::erase (m_children, child->undo_id ());
}

void abstract_svg_item::adopt_orphan (abstract_svg_item *child, int index)
{
  register_item_change ();
  child->register_item_change ();
  signal_layout_changed ();
  child->m_parent = undo_id ();
  if (index < 0)
    index = (int)m_children.size ();

  m_children.insert (m_children.begin () + index, child->undo_id ());
}

void abstract_svg_item::move_child (int position, abstract_svg_item *child)
{
  register_item_change ();
  int prev_pos = child->child_index ();
  auto it_child = m_children.begin () +  prev_pos;
  slide (it_child, it_child + 1, m_children.begin () + position);
  signal_child_moved (child->name (), prev_pos, position);
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

  attribute = computed->clone ();
  attribute->set_edit_handler (m_document->items_edit_handler ());
  add_attribute (attribute);
  return attribute;
}

void abstract_svg_item::signal_child_inserted (const string &child, int position)
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->child_added (name (), child, position); });
  signal_item_changed (item_change_type::MYSELF, false);
}

void abstract_svg_item::signal_child_removed (const string &child_name, int pos)
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->child_removed (name (), child_name, pos); });
  signal_item_changed (item_change_type::MYSELF, false);
}

void abstract_svg_item::signal_item_removed ()
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->item_removed (name ()); });
}

void abstract_svg_item::signal_child_moved (const string &child_name, int old_pos, int new_pos)
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->child_moved (name (), child_name, old_pos, new_pos); });
  signal_item_changed (item_change_type::MYSELF, false);
}

void abstract_svg_item::signal_attribute_change_start (const abstract_attribute *attribute)
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->attribute_change_start (name (), attribute); });
}

void abstract_svg_item::signal_attribute_change_end (const abstract_attribute *attribute)
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->attribute_change_end (name (), attribute); });
  signal_item_changed (item_change_type::MYSELF, true);
}

void abstract_svg_item::signal_layout_changed ()
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->layout_changed (name ()); });
  signal_item_changed (item_change_type::MYSELF, false);
}

void abstract_svg_item::signal_item_changed (item_change_type type, bool update_children)
{
  send_to_listeners ([&] (svg_item_observer * observer) { observer->item_changed (name (), type); });
  if (type != item_change_type::PARENT)
    {
      if (parent ())
        parent ()->signal_item_changed (item_change_type::CHILD, false);
    }
  if (type != item_change_type::CHILD && update_children)
    {
      for (auto &&child : *this)
        child->signal_item_changed (item_change_type::PARENT, true);
    }
}

abstract_state_t *abstract_svg_item::create_state ()
{
  svg_item_state *state = new svg_item_state;
  state->m_document = m_document;
  state->m_attributes = m_attributes;
  state->m_original_id = m_original_id;
  state->m_own_id = m_own_id;
  state->m_children = m_children;
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
      prepare_to_remove ();
      document ()->items_edit_handler ()->set_item_removed (name ());
      return;
    }

  const svg_item_state *state = static_cast<const svg_item_state *> (abstract_state);
  m_document = state->m_document;
  m_attributes = state->m_attributes;
  m_original_id = state->m_original_id;
  m_own_id = state->m_own_id;
  m_parent = state->m_parent;
  auto changed_items = document ()->items_edit_handler ();
  m_children = state->m_children;

  if (state->m_observers.empty ())
    FREE (m_observers);
  else
    {
      if (!m_observers)
        m_observers = new vector<int>;
      *m_observers = state->m_observers;
    }

  if (state->m_created_observers.empty ())
    FREE (m_created_observers);
  else
    {
      if (!m_created_observers)
        m_created_observers = new vector<int>;
      *m_created_observers = state->m_created_observers;
    }

  add_to_container ();
  changed_items->set_item_changed (name ());
  changed_items->set_item_layout_changed (name ());
  signal_item_changed (item_change_type::MYSELF, false);
}

void abstract_svg_item::observe_item (abstract_svg_item *item_to_observe, svg_item_observer *observer)
{
  if (!m_created_observers)
    m_created_observers = new vector<int>;

  item_to_observe->add_observer (observer);
  m_created_observers->push_back (observer->undo_id ());
}

void abstract_svg_item::add_observer (svg_item_observer *observer)
{
  if (!m_observers)
    m_observers = new vector<int>;

  int id = m_document->get_undo_handler ()->add_item (observer);
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
  return static_cast<abstract_attribute *> (m_document->get_undo_handler ()->get_item (id));
}

bool abstract_svg_item::has_attribute (const string &type_name) const
{
  return m_attributes.find (type_name) != m_attributes.end ();
}

void abstract_svg_item::send_to_listeners (std::function< void (svg_item_observer *)> func)
{
  if (!m_document->signals_enabled ())
    return;

  func (m_document->items_edit_handler ());
  auto container = m_document->get_undo_handler ();
  if (m_observers)
    {
      auto observers = *m_observers;
      for (int id : observers)
        {
          svg_item_observer *observer = static_cast<svg_item_observer *> (container->get_item (id));
          if (!observer)
            continue;

          func (observer);
        }
    }
}

vector<const abstract_attribute *> abstract_svg_item::attributes_list () const
{
  vector<const abstract_attribute *> result;
  for (auto & attribute : m_attributes)
    result.push_back (get_attribute_by_id (attribute.second));

  return result;
}

void abstract_svg_item::prepare_to_remove ()
{
  remove_from_container ();
  auto handler = m_document->get_undo_handler ();
  for (auto & attribute : m_attributes)
    handler->remove_item (attribute.second);

  if (m_observers)
    {
      for (int id : *m_observers)
        handler->remove_item (id);
    }

  if (m_created_observers)
    {
      for (int id : *m_created_observers)
        handler->remove_item (id);
    }
}

void abstract_svg_item::update_name ()
{
  if (!has_name ())
    {
      create_unique_name ();
      add_to_container ();
    }
}

abstract_svg_item::iterator abstract_svg_item::begin ()
{
  return abstract_svg_item::iterator (m_children.begin (), m_document->get_undo_handler ());
}

abstract_svg_item::iterator abstract_svg_item::end ()
{
  return abstract_svg_item::iterator (m_children.end (), m_document->get_undo_handler ());
}

abstract_svg_item::const_iterator abstract_svg_item::begin () const
{
  return abstract_svg_item::const_iterator (m_children.begin (), m_document->get_undo_handler ());
}

abstract_svg_item::const_iterator abstract_svg_item::end () const
{
  return abstract_svg_item::const_iterator (m_children.end (), m_document->get_undo_handler ());
}


void abstract_svg_item::replace_item (abstract_svg_item *item, bool copy_clones)
{
  set_undo_id (item->undo_id ());
  if (copy_clones)
    {
      m_original_id = item->m_original_id;
      m_own_id = item->m_own_id;
    }

  m_children = item->m_children;
  m_parent = item->m_parent;
  undo_handler *handler = document ()->get_undo_handler ();
  std::string cur_name = item->name ();
  if (item->m_observers)
    m_observers = new vector<int> (std::move (*item->m_observers));
  if (item->m_created_observers && copy_clones)
    m_created_observers = new vector<int> (std::move (*item->m_created_observers));
  item->prepare_to_remove ();
  handler->remove_item (item);
  m_document->get_undo_handler ()->add_item (this);
  handler->register_item (this);
  if (!is_cloned ())
    {
      get_attribute_for_change<svg_attribute_id> ()->set_id (cur_name);
      register_item_name ();
    }
  else
    add_to_container ();

  send_to_listeners ([&] (svg_item_observer * observer) { observer->item_replaced (name ()); });
}

const  abstract_attribute * abstract_svg_item::get_computed_attribute_by_type (svg_attribute_type type) const
{
  unique_ptr<abstract_attribute> tmp_attr (svg_attribute_factory::get ()->create_attribute (undo_id (), type));
  return  get_computed_attribute (tmp_attr->type_name (), tmp_attr->inherit_type (), tmp_attr->type ());
}

void abstract_svg_item::erase_created_observers ()
{
  auto handler = m_document->get_undo_handler ();
  if (m_created_observers)
    {
      for (int id : *m_created_observers)
        handler->remove_item (id);
    }
}

void abstract_svg_item::rename (std::string new_name)
{
  svg_items_container *container = document ()->item_container ();
  if (container->get_item (name ()))
    container->remove_item (this);
  m_own_id = new_name;
  container->add_item (this);
}

abstract_svg_item *abstract_svg_item::iterator::operator* ()
{
  return static_cast<abstract_svg_item *> (m_undo_handler->get_item (*m_it));
}

const abstract_svg_item *abstract_svg_item::const_iterator::operator* ()
{
  return static_cast<const abstract_svg_item *> (m_undo_handler->get_item (*m_it));
}

