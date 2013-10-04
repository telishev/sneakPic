#ifndef SELECTOR_TOOL_H
#define SELECTOR_TOOL_H

#include "editor/tools/abstract_tool.h"

class mouse_shortcuts_handler;
class svg_painter;
class rubberband_selection;

class selector_tool : public abstract_tool
{
  mouse_shortcuts_handler *m_mouse_handler;
  rubberband_selection    *m_rubberband;
  svg_painter             *m_painter;
public:
  selector_tool (svg_painter *painter);
  ~selector_tool ();

  virtual void activate () override;
  virtual void deactivate () override;
  virtual unsigned int mouse_event (const mouse_event_t &m_event) override;

  virtual void items_changed () override;

private:
  void start_rubberband_selection (const QPoint &pos);
  void move_rubberband_selection (const QPoint &pos);
  void end_rubberband_selection (const mouse_event_t &event);
  
};

#endif // SELECTOR_TOOL_H
