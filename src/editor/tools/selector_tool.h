#ifndef SELECTOR_TOOL_H
#define SELECTOR_TOOL_H

#include "editor/tools/abstract_tool.h"

class mouse_shortcuts_handler;
class svg_painter;
class rubberband_selection;
class items_move_handler;
class overlay_renderer;

class selector_tool : public abstract_tool
{
  rubberband_selection    *m_rubberband;
  items_move_handler      *m_move_handler;

public:
  selector_tool (svg_painter *painter);
  ~selector_tool ();

private:
  bool start_moving_object (const QPoint &pos);
  bool move_object (const QPoint &pos);
  bool end_moving_object (const QPoint &pos);
};

#endif // SELECTOR_TOOL_H
