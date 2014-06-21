#ifndef ADD_ITEM_OPERATION_H
#define ADD_ITEM_OPERATION_H

class abstract_svg_item;
class svg_painter;

class add_item_operation
{
  svg_painter *m_painter;
  bool m_clear_selection = true;
  bool m_apply_style = true;
  bool m_insert_item = true;
public:
  add_item_operation (svg_painter *painter, bool clear_selection = true);
  ~add_item_operation ();

  void set_clear_selection (bool clear) { m_clear_selection = clear; }
  void set_apply_style (bool apply_style) { m_apply_style = apply_style; }
  void set_insert_item (bool insert_item) { m_insert_item = insert_item; }

  bool apply (abstract_svg_item *item); // if apply is not successful please do not add any garbage to undo queue.
};

#endif // ADD_ITEM_OPERATION_H
