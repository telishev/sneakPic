#include "editor/path_anchor_handle.h"

#include <QPointF>
#include <QRect>
#include <QTransform>
#include <QColor>


#include "common/debug_utils.h"

#include "path/svg_path.h"

#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/items/svg_item_path.h"
#include "path_handles_editor.h"
#include "common/memory_deallocation.h"
#include "operations/path_edit_operation.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "path_handles_selection.h"


path_anchor_handle::path_anchor_handle (path_handles_editor *editor, svg_item_path *item, svg_path_geom_iterator path_it)
  : base_anchor_handle (path_it, item)
{
  m_editor = editor;
}

path_anchor_handle::~path_anchor_handle ()
{
}

bool path_anchor_handle::start_drag (QPointF local_pos)
{
  m_edit_operation.reset (new path_edit_operation (m_item));
  m_drag_start = local_pos;
  m_editor->handles_selection ()->clear ();
  m_editor->handles_selection ()->add_anchor (m_item->name (), point_id ());
  return true;
}

bool path_anchor_handle::drag (QPointF local_pos)
{
  m_drag_cur = local_pos;
  move_point ();
  m_editor->update ();
  return true;
}

bool path_anchor_handle::end_drag (QPointF local_pos)
{
  m_drag_cur = local_pos;
  apply_drag ();
  m_drag_cur = m_drag_start = QPointF ();
  m_editor->update ();
  return true;
}

void path_anchor_handle::interrupt_drag ()
{
  m_drag_cur = m_drag_start;
  move_point ();
  m_edit_operation.reset ();
  m_editor->update ();
}

QPointF path_anchor_handle::get_handle_center () const
{
  if (m_drag_start != m_drag_cur)
    return m_drag_cur;

  return base_anchor_handle::get_handle_center ();
}

void path_anchor_handle::apply_drag ()
{
  move_point ();
  m_editor->begin_changes ();
  m_edit_operation.reset ();
  m_editor->end_changes ("Move Node");
}

void path_anchor_handle::move_point ()
{
  m_edit_operation->get_svg_path ()->move_anchor (m_drag_cur, m_path_it);
}

string path_anchor_handle::item_name () const
{
  return m_item->name ();
}

const svg_path_geom *path_anchor_handle::get_path () const
{
  auto path_data = m_item->get_computed_attribute<svg_attribute_path_data> ();
  return path_data->path ();
}

node_type_t path_anchor_handle::node_type () const
{
  auto nodetypes = m_item->get_computed_attribute<svg_attribute_nodetypes> ();
  return nodetypes->node_type ((int)m_path_it.point_index ());
}

QTransform path_anchor_handle::get_path_transform () const
{
  return m_item->full_transform ();
}
