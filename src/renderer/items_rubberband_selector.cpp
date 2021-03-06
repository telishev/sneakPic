#include "items_rubberband_selector.h"

#include "editor/items_selection.h"

#include "renderer/svg_painter.h"

#include "gui/mouse_shortcut.h"
#include "gui/shortcuts_config.h"

#include "svg/items/svg_items_container.h"
#include "editor/operations/get_items_for_rect.h"

items_rubberband_selector::items_rubberband_selector (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier)
  : rubberband_selection (overlay, painter, applier, mouse_drag_shortcut_t::RUBBERBAND_SELECTION)
{
  set_end_func (std::bind (&items_rubberband_selector::end_selection_func, this, std::placeholders::_1));
}

items_rubberband_selector::~items_rubberband_selector ()
{

}

bool items_rubberband_selector::end_selection_func (const mouse_event_t &m_event)
{
  items_selection *selection = painter ()->selection ();
  if (m_event.modifier () != keyboard_modifier::SHIFT)
    selection->clear ();

  for (auto item : get_items_for_rect_operation (painter ()->document ()).apply (selection_rect ()))
    selection->add_item (item);
  return true;
}


