#include "editor/path_handles_editor.h"

#include "common/memory_deallocation.h"

#include "editor/element_handles.h"
#include "editor/path_anchor_handle.h"
#include "editor/path_preview_handle.h"
#include "editor/path_handles_selection.h"

#include "path/svg_path.h"

#include "svg/items/svg_item_path.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/items/svg_item_type.h"

#include "renderer/svg_painter.h"
#include "gui/mouse_shortcuts_handler.h"
#include "path_control_point_handle.h"
#include "svg/svg_document.h"

class path_elements_handles : public element_handles
{
  std::vector<abstract_handle *> m_handles;
  svg_path *m_path;
  svg_item_path *m_path_item;
  path_handles_editor *m_editor;

public:
  path_elements_handles (path_handles_editor *editor, svg_item_path *path_item)
  {
    m_editor = editor;
    m_path_item = path_item;
    m_path = nullptr;
    auto path_data = path_item->get_computed_attribute<svg_attribute_path_data> ();
    if (!path_data->is_empty ())
      {
        m_path = new svg_path (*path_data->path ());
        int total_anchors = (int)m_path->total_points ();

        m_handles.push_back (new path_preview_handle (path_item, m_path));

        for (int i = 0; i < total_anchors; i++)
          add_controls (i);

        for (int i = 0; i < total_anchors; i++)
          m_handles.push_back (new path_anchor_handle (m_editor, m_path_item, i, m_path));
      }

  }

  virtual ~path_elements_handles ()
  {
    for (auto handle : m_handles)
      {
        FREE (handle);
      }

    FREE (m_path);
  }


protected:
  virtual std::vector<abstract_handle *> handles () override
  {
    return m_handles;
  }

private:
  bool has_controls (int handle_id)
  {
    if (is_selected (handle_id))
      return true;

    if (is_selected ((int)m_path->prev_point (handle_id)))
      return true;

    if (is_selected ((int)m_path->next_point (handle_id)))
      return true;

    return false;
  }

  bool is_selected (int handle_id)
  {
    return m_editor->handles_selection ()->is_selected (m_path_item->name (), handle_id);
  }

  void add_controls (int handle_id)
  {
    if (!has_controls (handle_id))
      return;

    add_control (handle_id, false);
    add_control (handle_id, true);
  }

  void add_control (int handle_id, bool is_left)
  {
    if (m_path->control_point_exists (handle_id, is_left))
      m_handles.push_back (new path_control_point_handle (m_editor, m_path_item, handle_id, is_left, m_path));
  }


};


path_handles_editor::path_handles_editor (overlay_renderer *overlay, svg_painter *painter, mouse_shortcuts_handler *mouse_handler)
  : handles_editor (overlay, painter, mouse_handler)
{
  m_handles_selection = new path_anchors_selection (painter->selection (), painter->document ());

  ADD_SHORTCUT (m_mouse_handler, SELECT_HANDLE, return select_handle (m_event));
}

path_handles_editor::~path_handles_editor ()
{
  FREE (m_handles_selection);
}

element_handles *path_handles_editor::create_handles_for_item (abstract_svg_item *item)
{
  if (item->type () != svg_item_type::PATH)
    return nullptr;

  svg_item_path *path = static_cast<svg_item_path *> (item);
  return new path_elements_handles (this, path);
}

bool path_handles_editor::select_handle (const mouse_event_t &mevent)
{
  QPoint pos = mevent.pos ();
  abstract_handle *handle = get_handle_by_pos (pos);
  if (!handle)
    return false;

  path_anchor_handle *control_point = dynamic_cast<path_anchor_handle *> (handle);
  if (!control_point)
    return false;

  m_handles_selection->clear ();
  m_handles_selection->add_anchor (control_point->item_name (), control_point->point_id ());
  update_handles ();
  m_painter->update ();
  return true;
}

void path_handles_editor::begin_changes ()
{
  m_handles_selection->begin_changes ();
}

void path_handles_editor::end_changes ()
{
  m_painter->document ()->apply_changes ();
  m_handles_selection->end_changes ();
}

void path_handles_editor::update ()
{
  m_painter->update ();
}

void path_handles_editor::update_handles_impl ()
{
  m_handles_selection->update ();
}
