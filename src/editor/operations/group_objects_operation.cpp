#include "group_objects_operation.h"

#include "svg/items/svg_item_group.h"
#include "svg/svg_document.h"

#include "renderer/svg_painter.h"
#include "add_item_operation.h"
#include "svg/attributes/svg_attribute_transform.h"

group_objects_operation::group_objects_operation (svg_painter *painter)
{
  m_painter = painter;
}

abstract_svg_item * group_objects_operation::apply (const std::vector<abstract_svg_item *> &items)
{
  if (items.empty ())
    return nullptr;

  auto group = m_painter->document ()->create_new_svg_item<svg_item_group> ();
  abstract_svg_item *last_parent = nullptr;
  int position_to_insert = -1;
  QTransform new_group_transform = items.back ()->parent ()->to_graphics_item ()->full_transform ();
  for (auto &&item : items)
    {
      QTransform prev_transform = item->to_graphics_item ()->full_transform ();
      position_to_insert = item->child_index ();
      last_parent = item->parent ();
      item->parent ()->make_orphan (item);
      item->get_attribute_for_change<svg_attribute_transform> ()->set_transform (prev_transform * new_group_transform.inverted ());
      group->adopt_orphan (item);
    }

  last_parent->adopt_orphan (group, position_to_insert);
  add_item_operation op (m_painter);
  op.set_insert_item (false);
  op.set_apply_style (false);
  op.apply (group);
  return group;
}

