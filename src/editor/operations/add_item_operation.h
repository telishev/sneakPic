#ifndef ADD_ITEM_OPERATION_H
#define ADD_ITEM_OPERATION_H

class abstract_svg_item;
class svg_painter;

class add_item_operation
{
  svg_painter *m_painter;
  bool m_clear_selection;
public:
  add_item_operation (svg_painter *painter, bool clear_selection = true);
  ~add_item_operation ();

  void apply (abstract_svg_item *item);
};

#endif // ADD_ITEM_OPERATION_H
