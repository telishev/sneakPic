#include "svg/attributes/abstract_attribute.h"

#include "common/debug_utils.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"

#include "svg/svg_document.h"

#include "svg/attributes/svg_attribute_factory.h"
#include "svg/undoable_items_container.h"
#include "svg/simple_state_diff.h"
#include "svg/undo_handler.h"
#include "svg/changed_items_container.h"

class attribute_state : public abstract_state_t
{
  svg_document *m_document;
  int m_item;
  std::string m_saved_data;
  svg_attribute_type m_type;
  int m_undo_id;
public:
  attribute_state (svg_document *document, int item, svg_attribute_type type, const std::string &saved_data, int undo_id)
    : m_document (document), m_item (item), m_saved_data (saved_data), m_type (type), m_undo_id (undo_id) {}

  virtual undoable *create_new_item () override
  {
    abstract_attribute *attribute = m_document->attribute_factory ()->create_attribute (m_item, m_type);
    attribute->set_undo_id (m_undo_id);
    return attribute;
  }

  virtual abstract_state_diff_t *create_diff (const abstract_state_t *first, const abstract_state_t *second) override
  {
    return new simple_state_diff<attribute_state> (first, second);
  }

  attribute_state *clone () const
  {
    return new attribute_state (m_document, m_item, m_type, m_saved_data, m_undo_id);
  }

  int item_id () const { return m_item; }
  const char *saved_data () const { return m_saved_data.c_str (); }
};

abstract_attribute::abstract_attribute (svg_document *document)
{
  m_document = document;
  m_item_id = -1;
  m_is_inherited = false;
  if (m_document)
    m_document->get_undoable_items_container ()->assign_id (this);
}

abstract_attribute::~abstract_attribute ()
{
}

abstract_attribute *abstract_attribute::clone (svg_document *document) const
{
  DEBUG_ASSERT (m_item_id);

  abstract_attribute *result = document->attribute_factory ()->create_attribute (m_item_id, type ());
  DEBUG_ASSERT (result->type () == type ());

  QString saved_data;
  DEBUG_ASSERT (write (saved_data));
  DEBUG_ASSERT (result->read (saved_data.toUtf8 ().constData ()));
  return result; 
}

bool abstract_attribute::is_empty () const
{
  return m_document == nullptr;
}

abstract_svg_item * abstract_attribute::item () const
{
  if (!m_document)
    return nullptr;

  return static_cast<abstract_svg_item *>(m_document->get_undoable_items_container ()->get_item (m_item_id));
}

abstract_state_t *abstract_attribute::create_state ()
{
  QString saved_data;
  DEBUG_ASSERT (write (saved_data));
  return new attribute_state (m_document, m_item_id, type (), saved_data.toStdString (), undo_id ());
}

void abstract_attribute::load_from_state (const abstract_state_t *state)
{
  if (!state)
    {
      abstract_svg_item *svg_item = item ();
      if (svg_item)
        m_document->changed_items ()->set_item_changed (svg_item->name ());

      return;
    }

  const attribute_state *attr_state = static_cast<const attribute_state *> (state);
  DEBUG_ASSERT (read (attr_state->saved_data ()));
  set_item (attr_state->item_id ());
  abstract_svg_item *parent_item = item ();
  if (parent_item)
    document ()->changed_items ()->set_item_changed (parent_item->name ());
}

void abstract_attribute::set_item (int item_id)
{
  m_item_id = item_id;
}

void abstract_attribute::register_change ()
{
  auto handler = document ()->get_undo_handler ();
  handler->register_item (this);
}
