#ifndef RUBBERBAND_SELECTION_H
#define RUBBERBAND_SELECTION_H

#include "renderer/overlay_items_container.h"

class QRectF;
class QPointF;
class rubberband_renderer_item;
class svg_painter;
class actions_applier;
class mouse_event_t;

class rubberband_selection
{
  double m_start_x, m_start_y;
  double m_end_x, m_end_y;

  rubberband_renderer_item *m_render_item;
  svg_painter *m_painter;
  actions_applier *m_applier;
public:
  rubberband_selection (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  ~rubberband_selection ();
  
  bool start_selection (QPointF pos);
  bool move_selection (QPointF pos);
  bool end_selection (const mouse_event_t &event);

  QRectF selection_rect () const;
};

#endif // RUBBERBAND_SELECTION_H
