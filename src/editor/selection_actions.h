#ifndef SELECTION_ACTIONS_H
#define SELECTION_ACTIONS_H

class svg_painter;
class mouse_event_t;
class abstract_svg_item;

class selection_actions
{
  svg_painter *m_painter;
public:
  selection_actions (svg_painter *painter);
  ~selection_actions ();

private:
  bool select_item (const mouse_event_t &event);
  bool lower_object ();
  bool raise_object ();
  bool remove_items_in_selection ();
  bool select_prev_item (const mouse_event_t &event);
  const abstract_svg_item *get_lowest_selected_item () const;
};

#endif // SELECTION_ACTIONS_H
