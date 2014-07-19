#ifndef SELECTION_HELPERS_H
#define SELECTION_HELPERS_H

class items_selection;
class abstract_svg_item;

class selection_helpers
{
  items_selection *m_selection;
public:
  selection_helpers (items_selection *selection);

  bool select (const abstract_svg_item *item, bool add_object);

};

#endif // SELECTION_HELPERS_H
