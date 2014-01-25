#include "editor/pen_handles.h"

#include <QTransform>

#include "svg/items/svg_item_path.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_type.h"

#include "editor/element_handles.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "path/svg_path.h"
#include "base_anchor_handle.h"


pen_handles::pen_handles (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier)
  : handles_editor (overlay, painter, applier)
{
  m_new_path = 0;
}

pen_handles::~pen_handles ()
{

}

element_handles *pen_handles::create_handles_for_item (abstract_svg_item *item)
{
  if (item->type () != svg_item_type::PATH)
    return nullptr;

  svg_item_path *path_item = static_cast<svg_item_path *> (item);
  std::vector<abstract_handle *> handles;
  auto path_data = path_item->get_computed_attribute<svg_attribute_path_data> ();
  svg_path *path = path_data->path ();
  const auto &subpath = path->subpath ();
  for (size_t i = 0; i < subpath.size (); i++)
    {
      if (subpath[i].is_closed ())
        continue;

      auto first = subpath[i].begin ();
      auto last = subpath[i].last_point ();
      handles.push_back (new base_anchor_handle (svg_path_iterator (*path, i, first), path_item));
      if (last != first)
        handles.push_back (new base_anchor_handle (svg_path_iterator (*path, i, last), path_item));
    }

  return new element_handles_vector (handles); 
}

bool pen_handles::get_path_by_pos (QPointF screen_pos, svg_path_iterator &it, svg_item_path *&path)
{
  abstract_handle *handle = get_handle_by_pos (screen_pos);
  base_anchor_handle *base_handle = dynamic_cast<base_anchor_handle *> (handle);
  if (!base_handle)
    return false;

  it = base_handle->path_iterator ();
  path = base_handle->item ();
  return true;
}

void pen_handles::update_handles_impl ()
{
  if (!m_new_path)
    return;

  std::vector<abstract_handle *> handles;
  handles.push_back (new base_anchor_handle (m_new_path->begin (), nullptr));
  set_handle_for_item ("#new_path", new element_handles_vector (handles));
}

void pen_handles::set_new_path (svg_path *new_path)
{
  m_new_path = new_path;
  update_handles ();
}
