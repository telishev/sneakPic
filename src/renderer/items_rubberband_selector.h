#ifndef ITEMS_RUBBERBAND_SELECTOR_H
#define ITEMS_RUBBERBAND_SELECTOR_H

#include "renderer/rubberband_selection.h"

class items_rubberband_selector : public rubberband_selection
{
public:
  items_rubberband_selector (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  ~items_rubberband_selector ();

private:
  bool end_selection_func (const mouse_event_t &m_event);
};

#endif // ITEMS_RUBBERBAND_SELECTOR_H

