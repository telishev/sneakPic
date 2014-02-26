#include "svg/attributes/abstract_attribute.h"

#include "common/debug_utils.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"

#include "svg/svg_document.h"

#include "svg/attributes/svg_attribute_factory.h"
#include "svg/simple_state_diff.h"
#include "svg/undo/undo_handler.h"
#include "svg/changed_items_container.h"

class attribute_state : public abstract_state_t
{
  items_edit_handler_t *m_edit_handler;
  int m_item;
  string m_saved_data;
  svg_attribute_type m_type;
public:
  attribute_state (items_edit_handler_t *edit_handler, int item, svg_attribute_type type, const string &saved_data)
    : m_edit_handler (edit_handler), m_item (item), m_saved_data (saved_data), m_type (type) {}

  virtual undoable *create_new_item () override
  {
    abstract_attribute *attribute = svg_attribute_factory::get ()->create_attribute (m_item, m_type);
    return attribute;
  }

  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override
  {
    return new simple_state_diff<attribute_state> (first, second);
  }

  attribute_state *clone () const
  {
    return new attribute_state (m_edit_handler, m_item, m_type, m_saved_data);
  }

  items_edit_handler_t *edit_handler () const { return m_edit_handler; }
  int item_id () const { return m_item; }
  const char *saved_data () const { return m_saved_data.c_str (); }
};

abstract_attribute::abstract_attribute ()
{
  m_edit_handler = nullptr;
  m_item_id = -1;
  m_is_inherited = false;
}

abstract_attribute::~abstract_attribute ()
{
}

abstract_attribute *abstract_attribute::clone () const
{
  DEBUG_ASSERT (m_item_id);

  abstract_attribute *result = svg_attribute_factory::get ()->create_attribute (m_item_id, type ());
  result->set_edit_handler (m_edit_handler);
  DEBUG_ASSERT (result->type () == type ());

  QString saved_data;
  DEBUG_ASSERT (write (saved_data));
  DEBUG_ASSERT (result->read (saved_data.toUtf8 ().constData ()));
  return result; 
}

bool abstract_attribute::is_empty () const
{
  return m_edit_handler == nullptr;
}

abstract_svg_item * abstract_attribute::item () const
{
  if (!m_edit_handler)
    return nullptr;

  return static_cast<abstract_svg_item *>(m_edit_handler->get_undo_handler ()->get_item (m_item_id));
}

abstract_state_t *abstract_attribute::create_state ()
{
  QString saved_data;
  DEBUG_ASSERT (write (saved_data));
  return new attribute_state (m_edit_handler, m_item_id, type (), saved_data.toStdString ());
}

void abstract_attribute::load_from_state (const abstract_state_t *state)
{
  if (!state)
    {
      abstract_svg_item *svg_item = item ();
      if (svg_item)
        m_edit_handler->set_item_changed (svg_item->name ());

      return;
    }

  const attribute_state *attr_state = static_cast<const attribute_state *> (state);
  m_edit_handler = attr_state->edit_handler ();
  DEBUG_ASSERT (read (attr_state->saved_data ()));
  set_item (attr_state->item_id ());
  abstract_svg_item *parent_item = item ();
  if (parent_item)
    m_edit_handler->set_item_changed (parent_item->name ());
}

void abstract_attribute::set_item (int item_id)
{
  m_item_id = item_id;
}

void abstract_attribute::register_change ()
{
  auto handler = m_edit_handler->get_undo_handler ();
  handler->register_item (this);
}
