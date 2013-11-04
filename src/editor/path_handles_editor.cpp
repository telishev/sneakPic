#include "editor/path_handles_editor.h"

#include "common/memory_deallocation.h"

#include "editor/element_handles.h"
#include "editor/path_control_point.h"

#include "path/svg_path.h"

#include "svg/items/svg_item_path.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/items/svg_item_type.h"

class path_elements_handles : public element_handles
{
  svg_item_path *m_path;
  std::vector<abstract_handle *> m_handles;

public:
  path_elements_handles (svg_painter *painter, svg_item_path *path)
  {
    m_path = path;

    auto path_data = m_path->get_computed_attribute<svg_attribute_path_data> ();
    if (!path_data->is_empty ())
      {
        int total_handles = (int)path_data->path ()->total_points ();

        m_handles.resize (total_handles);
        for (int i = 0; i < total_handles; i++)
          m_handles[i] = new path_control_point (painter, m_path, i);
      }

  }

  virtual ~path_elements_handles ()
  {
    for (auto handle : m_handles)
      {
        FREE (handle);
      }
  }


protected:
  virtual std::vector<abstract_handle *> handles () override
  {
    return m_handles;
  }

};


path_handles_editor::path_handles_editor (overlay_renderer *overlay, svg_painter *painter, mouse_shortcuts_handler *mouse_handler)
  : handles_editor (overlay, painter, mouse_handler)
{

}

path_handles_editor::~path_handles_editor ()
{

}

element_handles *path_handles_editor::create_handles_for_item (abstract_svg_item *item)
{
  if (item->type () != svg_item_type::PATH)
    return nullptr;

  svg_item_path *path = static_cast<svg_item_path *> (item);
  return new path_elements_handles (m_painter, path);
}
