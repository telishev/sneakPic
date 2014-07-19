#include "editor/items_selection.h"

#include <QRectF>

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/items/svg_items_container.h"
#include "svg/attributes/svg_attribute_locked.h"

items_selection::items_selection (svg_items_container *container)
{
  m_container = container;
  m_bbox_invalid = true;
}

items_selection::~items_selection ()
{

}

void items_selection::add_item (const abstract_svg_item *item)
{
  return add_item (item->name ());
}

void items_selection::add_item (const string &item_name)
{
  m_selection.insert (item_name);
  m_bbox_invalid = true;
  emit selection_changed ();
}

void items_selection::remove_item (const string &item_name)
{
  m_selection.erase (item_name);
  m_bbox_invalid = true;
  emit selection_changed ();
}

void items_selection::remove_item (const abstract_svg_item *item)
{
  return remove_item (item->name ());
}

void items_selection::clear ()
{
  m_selection.clear ();
  emit selection_changed ();
}

bool items_selection::contains (const string &name) const
{
  return m_selection.find (name) != m_selection.end ();
}

int items_selection::count () const
{
  return (int) m_selection.size ();
}

void items_selection::remove_unavailable_items ()
{
  for (auto it = m_selection.begin (); it != m_selection.end ();)
    {
      if (m_container->contains (*it))
        ++it;
      else
        it = m_selection.erase (it);
    }

  emit selection_changed ();
}

abstract_svg_item * items_selection::single_item () const
{
  if (count () != 1)
    return nullptr;

  return m_container->get_item (*m_selection.begin ());
}

abstract_svg_item *items_selection::iterator::operator* ()
{
  return m_container->get_item (*m_it);
}

abstract_svg_item * items_selection::const_iterator::operator* ()
{
  return m_container->get_item (*m_it);
}

QRectF items_selection::get_bbox () const
{
  m_bbox = QRectF ();
  for (auto && item : *this)
    {
      auto graphics_item = item->to_graphics_item ();
      if (graphics_item != nullptr)
        m_bbox = m_bbox.united (graphics_item->bbox ());
    }
  m_bbox_invalid = false;
  return m_bbox;

}
