#include "svg_named_item.h"

#include "svg/attributes/svg_attribute_id.h"
#include "svg/svg_document.h"
#include "svg/items/svg_named_items_container.h"

svg_named_item::svg_named_item (svg_document *document)
  : abstract_svg_item (document)
{
  m_id = 0;
}

svg_named_item::~svg_named_item ()
{
  remove_from_container ();
}

QString svg_named_item::id () const
{
  if (!has_id ())
    return QString ();

  return m_id->id ();
}

void svg_named_item::set_id (const QString &id)
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


void svg_named_item::process_attribute (abstract_attribute *attribute)
{
  if (attribute->type () != svg_attribute_type::ID)
    return;

  m_id = static_cast<svg_attribute_id *> (attribute);
  add_to_container ();

  return abstract_svg_item::process_attribute (attribute);
}

void svg_named_item::add_to_container ()
{
  if (has_id ())
    document ()->item_container ()->add_item (this);
}

void svg_named_item::remove_from_container ()
{
  if (has_id ())
    document ()->item_container ()->remove_item (this);
}

