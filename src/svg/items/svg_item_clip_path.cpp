#include "svg/items/svg_item_clip_path.h"

#include <QPainterPath>

#include "svg/items/svg_item_type.h"
#include "svg/items/svg_base_shape_item.h"
#include "svg/items/svg_item_use.h"

#include "svg/attributes/svg_attribute_clip_path.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/svg_document.h"

svg_item_clip_path::svg_item_clip_path (svg_document *document)
  : abstract_svg_item (document)
{

}

svg_item_clip_path::~svg_item_clip_path ()
{

}

QPainterPath svg_item_clip_path::get_clip_path () const
{
  QPainterPath path;
  for (int i = 0; i < children_count (); i++)
    {
      const abstract_svg_item *cur_child = child (i);
      if (cur_child->type () == svg_item_type::USE)
        {
          QPainterPath child_path = get_single_child_path (cur_child->child (0));
          const svg_attribute_transform *transform = cur_child->get_computed_attribute<svg_attribute_transform> ();
          child_path = transform->computed_transform ().map (child_path);
          path.addPath (child_path);
        }
      else
        path.addPath (get_single_child_path (cur_child));
    }

  const svg_attribute_clip_path *clip_path_attr = get_computed_attribute<svg_attribute_clip_path> ();
  const svg_item_clip_path *clip_path = clip_path_attr->clip_path (document ()->item_container ());
  if (clip_path)
    path = path.intersected (clip_path->get_clip_path ());

  return path;
}

bool svg_item_clip_path::check_item ()
{
  return true;
}

QPainterPath svg_item_clip_path::get_single_child_path (const abstract_svg_item *child) const
{
  const svg_base_shape_item *base_shape = dynamic_cast<const svg_base_shape_item *> (child);
  if (!base_shape)
    return QPainterPath ();

  QPainterPath path = base_shape->get_path_for_clipping ();
  const svg_attribute_clip_path *clip_path_attr = child->get_computed_attribute<svg_attribute_clip_path> ();
  const svg_item_clip_path *clip_path = clip_path_attr->clip_path (document ()->item_container ());
  if (clip_path)
    path = path.intersected (clip_path->get_clip_path ());

  return path;
}
