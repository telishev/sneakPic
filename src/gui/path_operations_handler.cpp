#include "path_operations_handler.h"

#include "gui_action_id.h"
#include "actions_applier.h"

#include "renderer/svg_painter.h"

#include "editor/items_selection.h"
#include "editor/operations/object_to_path_operation.h"
#include "editor/operations/stroke_to_path_operation.h"
#include "editor/operations/path_bool_operation.h"

#include "svg/svg_document.h"
#include "svg/items/items_comparison.h"
#include "svg/items/svg_base_shape_item.h"
#include "svg/items/svg_item_path.h"




path_operations_handler::path_operations_handler (svg_painter *painter, actions_applier *applier)
{
  m_painter = painter;
  m_actions_applier = applier;
  m_actions_applier->register_action (gui_action_id::OBJECT_TO_PATH, this, &path_operations_handler::objects_to_path);
  m_actions_applier->register_action (gui_action_id::STROKE_TO_PATH, this, &path_operations_handler::strokes_to_path);
  m_actions_applier->register_action (gui_action_id::UNITE_PATH, this, &path_operations_handler::unite_path);
  m_actions_applier->register_action (gui_action_id::SUBPTRACT_PATH, this, &path_operations_handler::subtract_path);
  m_actions_applier->register_action (gui_action_id::INTERSECT_PATH, this, &path_operations_handler::intersect_path);
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

bool path_operations_handler::strokes_to_path ()
{
  return apply_for_selection ([&] (abstract_svg_item *item) {
    stroke_to_path_operation ().apply (item);
    return true;
  }, "Stroke to Path");
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

bool path_operations_handler::unite_path ()
{
  return path_bool_op (path_bool_operation_type::UNITE, "Unite");
}

bool path_operations_handler::intersect_path ()
{
  return path_bool_op (path_bool_operation_type::INTERSECT, "Intersection");
}

bool path_operations_handler::subtract_path ()
{
  return path_bool_op (path_bool_operation_type::SUBTRACT, "Difference");
}

bool path_operations_handler::path_bool_op (path_bool_operation_type op_type, QString undo_name)
{
  std::vector<abstract_svg_item *> items;
  for (auto &&item : *m_painter->selection ())
    {
      if (dynamic_cast<svg_base_shape_item *> (item))
        items.push_back (item);
    }

  if (items.size () < 2)
    return true;

  std::sort (items.begin (), items.end (), items_comparison_z_order ());
  m_painter->selection ()->clear ();

  abstract_svg_item *dst = items.front ();
  {
    path_bool_operation op (dst, op_type);
    if (!op.dst_path ())
      return true;

    for (auto &&src : items)
      if (src != dst)
        op.apply (src);

    m_painter->selection ()->add_item (op.dst_path ());
  }

  m_painter->document ()->apply_changes (undo_name);
  return true;
}
