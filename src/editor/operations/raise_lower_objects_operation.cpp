#include "raise_lower_objects_operation.h"
#include "svg/items/svg_graphics_item.h"

raise_lower_objects_operation::raise_lower_objects_operation ()
{
}

raise_lower_objects_operation::~raise_lower_objects_operation ()
{

}

void raise_lower_objects_operation::apply (const std::vector<abstract_svg_item *> &items, z_direction direction)
{
  // copy selection
  vector<svg_graphics_item *> selection_graphic_items;
  for (auto item : items)
    {
      auto graphics_item = item->to_graphics_item ();
      if (graphics_item)
        selection_graphic_items.push_back (graphics_item);
    }

  if (selection_graphic_items.size () == 0)
    return;

  abstract_svg_item *parent_item = selection_graphic_items[0]->parent ();

  if (!parent_item)
    return;

  for (auto && item : selection_graphic_items)
    {
      if (item->parent () != parent_item)
        return; // if parents are different - do nothing
    }

  // otherwise - sort by depth according to direction
  sort (selection_graphic_items.begin (), selection_graphic_items.end (), [&] (abstract_svg_item * itemA, abstract_svg_item * itemB)
  {
    bool less = (itemA->child_index () < itemB->child_index ());
    return (direction == z_direction::DOWN) ? !less : less;
  });
  // for example for down direction we are checking top-most item first:

  int limit_index = 0;

  // for each element finding element that lies between them and previous in the list which is appropriate for moving (intersects bounding box of our item)
  for (auto it = selection_graphic_items.begin (); it != selection_graphic_items.end (); ++it)
    {
      if (it + 1 == selection_graphic_items.end ())
        {
          switch (direction)
            {
            case z_direction::UP:
              limit_index = (int) parent_item->children_count ();
              break;
            case z_direction::DOWN:
              limit_index = -1;
              break;
            }
        }
      else
        limit_index = (* (it + 1))->child_index ();

      auto has_intersection = [&] (int index) {
                                                 svg_graphics_item *graphics_item = parent_item->child (index)->to_graphics_item ();
                                                 return graphics_item ? graphics_item->bbox ().intersects ((*it)->bbox ()) : false;
                                              };
      int found_index = -1;
      switch (direction)
        {
        case z_direction::UP:
          {
            for (int i = (*it)->child_index () + 1; i < limit_index; i++)
            {
              if (has_intersection (i))
              {
                found_index = i;
                break;
              }
            }
          }
          break;
        case z_direction::DOWN:
          {
            for (int i = (*it)->child_index () - 1; i > limit_index; i--)
            {

              if (has_intersection (i))
              {
                found_index = i;
                break;
              }
            }
          }
          break;
        }
      if (found_index != -1)
        {
          switch (direction)
            {
            case z_direction::UP:
              parent_item->move_child (found_index + 1, *it);
              break;
            case z_direction::DOWN:
              parent_item->move_child (found_index, *it);
              break;
            }

        }
    }
}
