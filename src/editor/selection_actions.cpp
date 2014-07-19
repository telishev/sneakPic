#include "editor/selection_actions.h"

#include "gui/mouse_filter.h"
#include "gui/shortcuts_config.h"
#include "gui/gui_action_id.h"
#include "gui/actions_applier.h"
#include "gui/canvas_widget_t.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/svg_document.h"
#include "svg/undo/undo_handler.h"

#include "renderer/svg_painter.h"

#include "editor/selection_helpers.h"
#include "editor/items_selection.h"

#include "editor/operations/raise_lower_objects_operation.h"
#include "operations/get_items_for_point_operation.h"
#include "item_helpers.h"
#include "common/range_algorithm.h"
#include "svg/items/items_comparison.h"


selection_actions::selection_actions (svg_painter *painter)
{
  m_painter = painter;
  actions_applier *applier = m_painter->get_actions_applier ();

  applier->add_shortcut (mouse_shortcut_t::SELECT_ITEM, this, &selection_actions::select_item);
  applier->register_action (gui_action_id::DELETE_ITEMS, this, &selection_actions::remove_items_in_selection);
  applier->register_action (gui_action_id::RAISE_OBJECT, this, &selection_actions::raise_object);
  applier->register_action (gui_action_id::LOWER_OBJECT, this, &selection_actions::lower_object);
  applier->add_shortcut (mouse_shortcut_t::SELECT_PREV_ITEM, this, &selection_actions::select_prev_item);

}

selection_actions::~selection_actions ()
{

}

bool selection_actions::select_item (const mouse_event_t &event)
{
  bool add_to_selection = contains_modifier (event.modifier (), keyboard_modifier::SHIFT);
  abstract_svg_item *item = m_painter->get_current_item (event.pos ());
  selection_type_t selection_type = m_painter->current_selection_type ();
  const abstract_svg_item *selectable_item = item_helpers (m_painter->document ()).get_selectable_item_or_group (item, selection_type);
  if (!selection_helpers (m_painter->selection ()).select (selectable_item, add_to_selection))
    return false;

  m_painter->canvas_widget ()->update ();
  return true;
}

bool selection_actions::lower_object ()
{
  items_selection *selection = m_painter->selection ();
  std::vector<abstract_svg_item *> selection_items (selection->begin (), selection->end ());
  raise_lower_objects_operation ().apply (selection_items, z_direction::DOWN);
  m_painter->document ()->apply_changes ("Lower");
  return true;
}

bool selection_actions::raise_object ()
{
  items_selection *selection = m_painter->selection ();
  std::vector<abstract_svg_item *> selection_items (selection->begin (), selection->end ());
  raise_lower_objects_operation ().apply (selection_items, z_direction::UP);
  m_painter->document ()->apply_changes ("Raise");
  return true;
}

bool selection_actions::remove_items_in_selection ()
{
  items_selection *selection = m_painter->selection ();
  if (selection->count () == 0)
    return true;

  for (auto item : *selection)
    {
      if (item && item->parent ())
        item->parent ()->remove_child (item);
    }

  selection->clear ();
  m_painter->document ()->apply_changes ("Remove");
  return true;
}

bool selection_actions::select_prev_item (const mouse_event_t &event)
{
  bool add_to_selection = contains_modifier (event.modifier (), keyboard_modifier::SHIFT);
  
  get_items_for_point_operation op (m_painter->document (), m_painter->current_selection_type ());
  QPointF local_pos = m_painter->get_local_pos (event.pos ());
  const abstract_svg_item *new_item = op.get_prev_item_for_point (get_lowest_selected_item (), local_pos);

  selection_helpers (m_painter->selection ()).select (new_item, add_to_selection);
  m_painter->canvas_widget ()->update ();
  return true;
}

const abstract_svg_item * selection_actions::get_lowest_selected_item () const
{
  return *range::min_element (*m_painter->selection (), items_comparison_z_order ());
}

