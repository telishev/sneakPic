#include "selection_helpers.h"

#include "items_selection.h"
#include "svg/items/abstract_svg_item.h"

selection_helpers::selection_helpers (items_selection *selection)
{
  m_selection = selection;
}

bool selection_helpers::select (const abstract_svg_item *item, bool add_object)
{
  if (item == nullptr)
    {
      if (!add_object && !m_selection->empty ())
        {
          m_selection->clear ();
          return true;
        }

      return false;
    }

  bool has_item = m_selection->contains (item->name ());
  if (has_item && !add_object)
    return false;

  if (!add_object)
    m_selection->clear ();

  if (has_item && add_object)
    m_selection->remove_item (item);
  else
    m_selection->add_item (item);

  return true;
}


