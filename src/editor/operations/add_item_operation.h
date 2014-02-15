#ifndef ADD_ITEM_OPERATION_H
#define ADD_ITEM_OPERATION_H

class abstract_svg_item;
class svg_painter;

class add_item_operation
{
  svg_painter *m_painter;
  bool m_clear_selection;
  bool m_apply_style;
public:
  add_item_operation (svg_painter *painter, bool clear_selection = true);
  ~add_item_operation ();

  void set_clear_selection (bool clear) { m_clear_selection = clear; }
  void set_apply_style (bool apply_style) { m_apply_style = apply_style; }

  void apply (abstract_svg_item *item);
};

#endif // ADD_ITEM_OPERATION_H
