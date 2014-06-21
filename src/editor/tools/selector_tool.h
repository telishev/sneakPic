#ifndef SELECTOR_TOOL_H
#define SELECTOR_TOOL_H

#include "editor/tools/abstract_tool.h"

class mouse_shortcuts_handler;
class svg_painter;
class rubberband_selection;
class items_move_handler;
class overlay_renderer;
class transform_handles_editor;
class mouse_event_t;
class QPoint;
class abstract_svg_item;

class selector_tool : public abstract_tool
{
  rubberband_selection    *m_rubberband;
  items_move_handler      *m_move_handler;
  unique_ptr <transform_handles_editor> m_transform_handles_editor;

public:
  selector_tool (svg_painter *painter);
  ~selector_tool ();

private:
  bool start_moving_object (const QPoint &pos);
  bool move_object (const QPoint &pos);
  bool end_moving_object (const QPoint &pos);
  virtual void activate () override;
  virtual void deactivate () override;

  void update_handles ();
  virtual void configure ();
  void change_handles_type ();
  bool select_object (const mouse_event_t &event);
  abstract_svg_item *get_item_or_group_for_pos (const QPoint &pos) const;
};

#endif // SELECTOR_TOOL_H
