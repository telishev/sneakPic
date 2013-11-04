#include "editor/items_selection.h"

#include <QRectF>

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/items/svg_items_container.h"

items_selection::items_selection (svg_items_container *container)
{
  m_container = container;
}

items_selection::~items_selection ()
{

}

void items_selection::add_item (const abstract_svg_item *item)
{
  return add_item (item->name ());
}

void items_selection::add_item (const std::string &item_name)
{
  m_selection.insert (item_name);
  emit selection_changed ();
}

void items_selection::remove_item (const std::string &item_name)
{
  m_selection.erase (item_name);
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

void items_selection::add_items_for_rect (const QRectF &rect, const abstract_svg_item *root)
{
  const svg_graphics_item *graphics_item = root->to_graphics_item ();
  if (!graphics_item)
    return;

  if (graphics_item->can_be_selected ())
    {
      if (!rect.contains (graphics_item->bbox ()))
        return;

      add_item (root);
      return;
    }

  if (!graphics_item->bbox ().intersects (rect))
    return;

  for (int i = 0; i < root->children_count (); i++)
    add_items_for_rect (rect, root->child (i));
}

bool items_selection::contains (const std::string &name) const
{
  return m_selection.find (name) != m_selection.end ();
}

items_selection::iterator items_selection::begin ()
{
  return items_selection::iterator (m_container, m_selection.begin ());
}

items_selection::iterator items_selection::end ()
{
  return items_selection::iterator (m_container, m_selection.end ());
}

int items_selection::selected_count () const
{
  return (int) m_selection.size ();
}

abstract_svg_item *items_selection::iterator::operator* ()
{
  return m_container->get_item (*m_it);
}

items_selection::iterator::iterator (svg_items_container *container, set_type::iterator it)
{
  m_container = container;
  m_it = it;
}

bool items_selection::iterator::operator!= (const iterator &other) const
{
  return (this->m_it != other.m_it);
}

items_selection::iterator &items_selection::iterator::operator++ ()
{
  this->m_it++;
  return *this;
}
