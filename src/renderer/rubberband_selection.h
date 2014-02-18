#ifndef RUBBERBAND_SELECTION_H
#define RUBBERBAND_SELECTION_H

#include "renderer/overlay_items_container.h"

#include <functional>

class QRectF;
class QPointF;
class rubberband_renderer_item;
class svg_painter;
class actions_applier;
class mouse_event_t;

enum class mouse_drag_shortcut_t;

class rubberband_selection
{
  double m_start_x, m_start_y;
  double m_end_x, m_end_y;

  rubberband_renderer_item *m_render_item;
  svg_painter *m_painter;
  actions_applier *m_applier;
  std::function<bool (const mouse_event_t &)> m_start_func, m_drag_func, m_end_func;

public:
  rubberband_selection (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier, mouse_drag_shortcut_t drag_shortcut);
  ~rubberband_selection ();

  svg_painter *painter () const { return m_painter; }

  void set_start_func (std::function<bool (const mouse_event_t &)> start_func) { m_start_func = start_func; }
  void set_drag_func (std::function<bool (const mouse_event_t &)> drag_func) { m_drag_func = drag_func; }
  void set_end_func (std::function<bool (const mouse_event_t &)> end_func) { m_end_func = end_func; }

  QRectF selection_rect () const;

private:
  bool start_selection (const mouse_event_t &m_event);
  bool move_selection (const mouse_event_t &m_event);
  bool end_selection (const mouse_event_t &m_event);

};

#endif // RUBBERBAND_SELECTION_H
