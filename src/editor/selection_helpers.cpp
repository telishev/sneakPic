#include "selection_helpers.h"

#include "items_selection.h"
#include "svg/items/abstract_svg_item.h"

selection_helpers::selection_helpers (items_selection *selection)
{
  m_selection = selection;
}

void selection_helpers::select (abstract_svg_item *item, bool add_object)
{
  if (!add_object)
    m_selection->clear ();

  if (item == nullptr)
    return;

  if (m_selection->contains (item->name ()) && add_object)
    m_selection->remove_item (item);
  else
    m_selection->add_item (item);
}


