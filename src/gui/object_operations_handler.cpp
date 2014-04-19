#include "object_operations_handler.h"

#include "actions_applier.h"
#include "gui_action_id.h"
#include "renderer/svg_painter.h"
#include "svg/svg_document.h"
#include "editor/items_selection.h"
#include "editor/operations/clone_item_operation.h"




object_operations_handler::object_operations_handler (svg_painter *painter, actions_applier *applier)
{
  m_painter = painter;
  m_actions_applier = applier;
  m_actions_applier->register_action (gui_action_id::CLONE, this, &object_operations_handler::clone_object);
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