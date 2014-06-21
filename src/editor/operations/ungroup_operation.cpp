#include "ungroup_operation.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "renderer/svg_painter.h"
#include "editor/items_selection.h"


ungroup_operation::ungroup_operation (svg_painter *painter)
{
  m_painter = painter;
}

void ungroup_operation::apply (abstract_svg_item *group)
{
  int index = group->child_index ();
  auto selection = m_painter->selection ();
  abstract_svg_item *parent = group->parent ();
  QTransform group_transform = group->get_computed_attribute <svg_attribute_transform> ()->computed_transform ();
  std::vector<abstract_svg_item *> children (group->begin (), group->end ());

  for (auto && child : children)
    {
      group->make_orphan (child);
      parent->adopt_orphan (child, index++);
      auto transform_attr = child->get_attribute_for_change<svg_attribute_transform> ();
      transform_attr->set_transform (transform_attr->computed_transform () * group_transform);
      selection->add_item (child->name ());
    }

  selection->remove_item (group->name ());
  parent->remove_child (group);
}
