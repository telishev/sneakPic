#include "object_operations_handler.h"

#include "actions_applier.h"
#include "gui_action_id.h"
#include "renderer/svg_painter.h"
#include "svg/svg_document.h"
#include "editor/items_selection.h"
#include "editor/operations/clone_item_operation.h"
#include "editor/operations/duplicate_item_operation.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_type.h"
#include "svg/items/svg_item_use.h"
#include "svg/items/items_comparison.h"
#include "svg/items/svg_base_shape_item.h"
#include "editor/operations/clip_operation.h"
#include "editor/operations/group_objects_operation.h"
#include "../editor/operations/ungroup_operation.h"




object_operations_handler::object_operations_handler (svg_painter *painter, actions_applier *applier)
{
  m_painter = painter;
  m_actions_applier = applier;
  m_actions_applier->register_action (gui_action_id::CLONE, this, &object_operations_handler::clone_object);
  m_actions_applier->register_action (gui_action_id::DUPLICATE_ITEM, this, &object_operations_handler::duplicate);
  m_actions_applier->register_action (gui_action_id::UNLINK_CLONE, this, &object_operations_handler::unlink);
  m_actions_applier->register_action (gui_action_id::CLIP_OBJECTS, this, &object_operations_handler::clip);
  m_actions_applier->register_action (gui_action_id::GROUP, this, &object_operations_handler::group);
  m_actions_applier->register_action (gui_action_id::UNGROUP, this, &object_operations_handler::ungroup);
}

object_operations_handler::~object_operations_handler ()
{

}

bool object_operations_handler::clone_object ()
{
  return apply_for_selection (
    [&] (abstract_svg_item *item) {
      return clone_item_operation (m_painter).apply (item) != nullptr;
  }, "Clone");
}

bool object_operations_handler::duplicate ()
{
  return apply_for_selection (
    [&] (abstract_svg_item *item) {
      return duplicate_item_operation (m_painter).apply (item) != nullptr;
  }, "Duplicate");
}

bool object_operations_handler::unlink ()
{
  items_selection *selection = m_painter->selection ();
  return apply_for_selection (
    [&] (abstract_svg_item *item) {
      if (item->type () != svg_item_type::USE)
        {
          selection->add_item (item);
          return false;
        }
      selection->add_item (static_cast<svg_item_use *> (item)->unlink ());
      return true;
  }, "Unlink");
}

bool object_operations_handler::apply_for_selection (std::function<bool (abstract_svg_item *)> func, QString undo_name)
{
  bool applied = false;
  std::vector<abstract_svg_item *> items (m_painter->selection ()->begin (), m_painter->selection ()->end ());
  m_painter->selection ()->clear ();
  for (auto &&item : items)
    {
      if (!item)
        continue;

      bool ret = func (item);
      if (ret)
        applied = true;
    }
  
  if (!applied)
    return false;

  m_painter->document ()->apply_changes (undo_name);
  return true;
}

bool object_operations_handler::clip ()
{
  std::vector<abstract_svg_item *> items  = items_in_selection ();
  auto clip_item_it = std::find_if (items.begin (), items.end (), [] (abstract_svg_item *item) 
    { return dynamic_cast<svg_base_shape_item *> (item) != nullptr || item->type () == svg_item_type::USE;});
  if (clip_item_it == items.end ())
    return true;

  abstract_svg_item *clip_item = *clip_item_it;
  items.erase (clip_item_it);

  clip_operation ().apply (clip_item, items);
  m_painter->document ()->apply_changes ("Set Clip");
  return true;
}

bool object_operations_handler::group ()
{
  std::vector<abstract_svg_item *> items  = items_in_selection ();
  group_objects_operation (m_painter).apply (items);
  m_painter->document ()->apply_changes ("Group");
  return true;
}

bool object_operations_handler::ungroup ()
{
  return apply_for_selection (
    [&] (abstract_svg_item *item) {
      ungroup_operation (m_painter).apply (item);
      return true;
  }, "Ungroup");
}

std::vector<abstract_svg_item *> object_operations_handler::items_in_selection () const
{
  items_selection *selection = m_painter->selection ();
  if (selection->empty ())
    return {};

  std::vector<abstract_svg_item *> items (selection->begin (), selection->end ());
  std::sort (items.begin (), items.end (), items_comparison_z_order ());
  return items;
}
