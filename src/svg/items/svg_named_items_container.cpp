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
  QString id = item->id ();
  DEBUG_ASSERT (!id.isEmpty ());
  DEBUG_ASSERT (!contains (id));

  m_map.insert (std::make_pair (id, item));
}

void svg_named_items_container::remove_item (svg_named_item *item)
{
  QString id = item->id ();
  DEBUG_ASSERT (!id.isEmpty ());
  DEBUG_ASSERT (contains (id));
  m_map.erase (id);
}

svg_named_item *svg_named_items_container::get_item (const QString &id) const
{
  auto it = m_map.find (id);
  if (it == m_map.end ())
    return 0;

  return it->second;
}

bool svg_named_items_container::contains (const QString &id) const
{
  return m_map.find (id) != m_map.end ();
}
