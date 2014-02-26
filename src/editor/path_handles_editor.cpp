#include "editor/path_handles_editor.h"

#include "common/memory_deallocation.h"

#include "editor/element_handles.h"
#include "editor/path_anchor_handle.h"
#include "editor/path_preview_handle.h"
#include "editor/path_handles_selection.h"
#include "editor/path_control_point_handle.h"
#include "editor/items_selection.h"

#include "path/svg_path_geom.h"

#include "svg/items/svg_item_path.h"
#include "svg/items/svg_item_type.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/svg_attribute_linetypes.h"
#include "svg/svg_document.h"

#include "renderer/rubberband_selection.h"
#include "renderer/svg_painter.h"

#include "gui/actions_applier.h"
#include "gui/shortcuts_config.h"
#include "gui/gui_action_id.h"
#include "operations/path_edit_operation.h"
#include "operations/anchor_type_change_operation.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "path/svg_path.h"
#include "operations/remove_anchors_operation.h"
#include "svg/items/svg_items_container.h"

class path_elements_handles : public element_handles
{
  vector<abstract_handle *> m_handles;
  vector<path_anchor_handle *> m_anchor_handles;

  const svg_path_geom *m_path;
  svg_item_path *m_path_item;
  path_handles_editor *m_editor;
  const svg_attribute_linetypes *m_line_types;

public:
  path_elements_handles (path_handles_editor *editor, svg_item_path *path_item)
  {
    m_editor = editor;
    m_path_item = path_item;
    m_path = nullptr;
    m_line_types = path_item->get_computed_attribute<svg_attribute_linetypes> ();
    auto path_data = path_item->get_computed_attribute<svg_attribute_path_data> ();
    if (!path_data->is_empty ())
      {
        m_path = path_data->path ();
        m_handles.push_back (new path_preview_handle (path_item));

        for (auto it = m_path->begin (); it != m_path->end (); ++it)
          add_controls (it);

        for (auto it = m_path->begin (); it != m_path->end (); ++it)
          {
            path_anchor_handle *handle = new path_anchor_handle (m_editor, m_path_item, it);
            handle->set_is_selected (is_selected (it));
            m_anchor_handles.push_back (handle);
            m_handles.push_back (handle);
          }
      }

  }

  virtual ~path_elements_handles ()
  {
    for (auto handle : m_handles)
      {
        FREE (handle);
      }
  }

  const vector<path_anchor_handle *> &anchor_handles () { return m_anchor_handles; }


protected:
  virtual vector<abstract_handle *> handles () override
  {
    return m_handles;
  }

private:
  bool has_controls (svg_path_geom_iterator it)
  {
    if (is_selected (it))
      return true;

    if (is_selected (it.left ()))
      return true;

    if (is_selected (it.right ()))
      return true;

    return false;
  }

  bool is_selected (svg_path_geom_iterator it)
  {
    return m_editor->handles_selection ()->is_selected (m_path_item->name (), (int)it.point_index ());
  }

  void add_controls (svg_path_geom_iterator it)
  {
    if (!has_controls (it))
      return;

    add_control (it, false);
    add_control (it, true);
  }

  void add_control (svg_path_geom_iterator it, bool is_left)
  {
    if (it.has_control_point (is_left))
      {
        int segment_id = it.segment_index (is_left);
        if (segment_id >= 0 && !m_line_types->is_line_segment (segment_id))
          m_handles.push_back (new path_control_point_handle (m_editor, m_path_item, it, is_left));
      }
  }


};


path_handles_editor::path_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier)
  : handles_editor (overlay, painter, applier)
{
  m_handles_selection = new path_anchors_selection (painter->selection (), painter->document ());
  m_rubberband = new rubberband_selection (overlay, painter, applier, mouse_drag_shortcut_t::HANDLES_SELECTION);

  m_rubberband->set_start_func ([=] (const mouse_event_t &)
    {
      return !painter->selection ()->empty ();
    });

  m_rubberband->set_end_func ([=] (const mouse_event_t &m_event)
    {
      if (m_event.modifier () != SHIFT)
        m_handles_selection->clear ();

      select_by_rect (m_rubberband->selection_rect ());
      return true;
    });

  m_applier->add_shortcut (mouse_shortcut_t::SELECT_HANDLE, this, &path_handles_editor::select_handle);
  m_applier->add_shortcut (mouse_shortcut_t::CHANGE_HANDLE_TYPE, this, &path_handles_editor::change_handle);
  m_applier->register_action (gui_action_id::DELETE_HANDLES, this, &path_handles_editor::delete_selected_handles);
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
  path_anchor_handle *control_point = get_path_anchor (pos);
  if (!control_point)
    return false;

  if (!contains_modifier (mevent.modifier (), SHIFT))
    m_handles_selection->clear ();
  m_handles_selection->add_anchor (control_point->item_name (), control_point->point_id ());
  update_handles ();
  update ();
  return true;
}

void path_handles_editor::begin_changes ()
{
  m_handles_selection->begin_changes ();
}

void path_handles_editor::end_changes (QString name)
{
  m_painter->document ()->apply_changes (name);
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

void path_handles_editor::select_by_rect (const QRectF &rect)
{
  for (const auto &handle : *this)
    {
      /// TODO: do it without dynamic_cast
      path_elements_handles *handles = dynamic_cast<path_elements_handles *> (handle.second.get ());
      if (!handles)
        continue;

      for (auto control_point : handles->anchor_handles ())
        {
          QPointF center = control_point->get_handle_center ();
          if (rect.contains (center))
            m_handles_selection->add_anchor (control_point->item_name (), control_point->point_id ());
        }
    }

  update_handles ();
  update ();
}

bool path_handles_editor::delete_selected_handles ()
{
  auto selected_anchors = m_handles_selection->selected_anchors ();
  if (selected_anchors.empty ())
    return false;

  {
    svg_items_container *container = m_painter->document ()->item_container ();
    for (auto &it : selected_anchors)
      {
        svg_item_path *item = dynamic_cast<svg_item_path *> (container->get_item (it.first));
        if (!item)
          continue;

        bool need_to_remove = false;
        {
          path_edit_operation edit_op (item);
          svg_path *svg_path = edit_op.get_svg_path ();
          remove_anchors_operation op (svg_path);
          set<svg_path_geom_iterator> anchors_for_item;
          for (int point_index : it.second)
            anchors_for_item.insert (svg_path->get_geom ()->point (point_index));

          op.apply (anchors_for_item);
          if (svg_path->get_geom ()->total_points () == 0)
            need_to_remove = true;
        }

        if (need_to_remove)
          {
            if (item->parent ())
              item->parent ()->remove_child (item);
          }
      }
  }

  m_handles_selection->clear ();
  m_painter->document ()->apply_changes ("Delete Handles");
  return true;
}

bool path_handles_editor::change_handle (const QPoint &pos)
{
  path_anchor_handle *control_point = get_path_anchor (pos);
  if (!control_point)
    return false;

  begin_changes ();
  {
    path_edit_operation op (control_point->item ());
    node_type_t node_type = node_type_t::CUSP;
    if ((*op.get_svg_path ()->get_node_type ())[control_point->path_iterator ().point_index ()] == node_type_t::CUSP)
      node_type = node_type_t::SMOOTH;
    anchor_type_change_operation (op.get_svg_path ()).apply (control_point->path_iterator (), node_type);
  }
  end_changes ("Change Node Type");
  return true;
}

path_anchor_handle *path_handles_editor::get_path_anchor (const QPoint &pos) const
{
  abstract_handle *handle = get_handle_by_pos (pos);
  if (!handle)
    return nullptr;

  return dynamic_cast<path_anchor_handle *> (handle);
}
