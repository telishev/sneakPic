#include "items_comparison.h"

#include "abstract_svg_item.h"


void items_comparison_z_order::fill_parents_list (const abstract_svg_item *item, std::vector<const abstract_svg_item *> &parents)
{
  for (const abstract_svg_item *parent = item; parent; parent = parent->parent ())
    parents.push_back (parent);

  std::reverse (parents.begin (), parents.end ());
}

bool items_comparison_z_order::operator () (const abstract_svg_item *first, const abstract_svg_item *second)
{
  std::vector<const abstract_svg_item *> first_parents, second_parents;
  fill_parents_list (first, first_parents);
  fill_parents_list (second, second_parents);

  size_t min_size = std::min (first_parents.size (), second_parents.size ());

  for (size_t i = 0; i < min_size; i++)
    {
      if (first_parents[i] == second_parents[i])
        continue;

      return first_parents[i]->child_index () < second_parents[i]->child_index ();
    }

  return false;
}

