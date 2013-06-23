#include "svg/items/svg_named_items_container.h"

#include "common/debug_utils.h"

#include "svg/items/svg_named_item.h"

svg_named_items_container::svg_named_items_container ()
{

}

svg_named_items_container::~svg_named_items_container ()
{

}

void svg_named_items_container::add_item (svg_named_item *item)
{
  QString name = item->name ();
  DEBUG_ASSERT (!name.isEmpty ());
  DEBUG_ASSERT (!contains (name));

  m_map.insert (std::make_pair (name, item));
}

void svg_named_items_container::remove_item (svg_named_item *item)
{
  QString name = item->name ();
  DEBUG_ASSERT (!name.isEmpty ());
  DEBUG_ASSERT (contains (name));
  m_map.erase (name);
}

svg_named_item *svg_named_items_container::get_item (const QString &name) const
{
  auto it = m_map.find (name);
  if (it == m_map.end ())
    return 0;

  return it->second;
}

bool svg_named_items_container::contains (const QString &name) const
{
  return m_map.find (name) != m_map.end ();
}
