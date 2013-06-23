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

QString svg_named_item::name () const
{
  if (!has_name ())
    return QString ();

  return m_id->id ();
}

void svg_named_item::set_name (const QString &name)
{
  remove_from_container ();
  if (!m_id)
    {
      m_id = new svg_attribute_id;
      add_attribute (m_id);
    }

  m_id->set_id (name);
  add_to_container ();
}


void svg_named_item::post_read ()
{
  abstract_svg_item::post_read ();

  for (abstract_attribute *attribute : m_attributes)
    {
      if (attribute->type () != attribute_type::ID)
        continue;

      m_id = static_cast<svg_attribute_id *> (attribute);
      add_to_container ();
      break;
    }
}

void svg_named_item::add_to_container ()
{
  if (has_name ())
    document ()->item_container ()->add_item (this);
}

void svg_named_item::remove_from_container ()
{
  if (has_name ())
    document ()->item_container ()->remove_item (this);
}

