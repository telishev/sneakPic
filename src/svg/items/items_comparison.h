#ifndef ITEMS_COMPARISON_H
#define ITEMS_COMPARISON_H

class abstract_svg_item;

class items_comparison_z_order
{
public:
  bool operator () (const abstract_svg_item *first, const abstract_svg_item *second);

private:
  void fill_parents_list (const abstract_svg_item *item, std::vector<const abstract_svg_item *> &parents);
};

#endif // ITEMS_COMPARISON_H
