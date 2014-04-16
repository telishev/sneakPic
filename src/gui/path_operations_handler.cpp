#include "path_operations_handler.h"

#include "gui_action_id.h"
#include "actions_applier.h"
#include "renderer/svg_painter.h"
#include "editor/items_selection.h"
#include "editor/operations/object_to_path_operation.h"
#include "svg/svg_document.h"




path_operations_handler::path_operations_handler (svg_painter *painter, actions_applier *applier)
{
  m_painter = painter;
  m_actions_applier = applier;
  m_actions_applier->register_action (gui_action_id::OBJECT_TO_PATH, this, &path_operations_handler::objects_to_path);
}

path_operations_handler::~path_operations_handler ()
{

}

bool path_operations_handler::objects_to_path ()
{
  return apply_for_selection ([&] (abstract_svg_item *item) {
    object_to_path_operation ().apply (item);
    return true;
  }, "Object to Path");
}

bool path_operations_handler::apply_for_selection (std::function<bool (abstract_svg_item *)> func, QString undo_name)
{
  bool applied = false;
  for (auto &&item : *m_painter->selection ())
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
