#include "editor/items_selection.h"

#include "svg/items/abstract_svg_item.h"

items_selection::items_selection (const svg_document *document)
{
  m_document = document;
}

items_selection::~items_selection ()
{

}

void items_selection::add_item (const abstract_svg_item *item)
{
  return add_item (item->name ().toStdString ());
}

void items_selection::add_item (const std::string &item_name)
{
  DEBUG_ASSERT (m_selection.find (item_name) == m_selection.end ());

  m_selection.insert (item_name);
}

void items_selection::clear ()
{
  m_selection.clear ();
}

