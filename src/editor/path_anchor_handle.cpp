#include "editor/path_anchor_handle.h"

#include <QPointF>
#include <QRect>
#include <QTransform>
#include <QColor>

#include "skia/skia_includes.h"

#include "common/debug_utils.h"


#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"

#include "path/svg_path.h"

#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/items/svg_item_path.h"
#include "path_handles_editor.h"
#include "common/memory_deallocation.h"
#include "operations/path_edit_operation.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "renderer/anchor_handle_renderer.h"


path_anchor_handle::path_anchor_handle (path_handles_editor *editor, svg_item_path *item, svg_path_iterator path_it)
{
  m_item = item;
  m_path_it = path_it;
  m_editor = editor;
  m_is_highlighted = false;
}

path_anchor_handle::~path_anchor_handle ()
{
}

int path_anchor_handle::distance_to_mouse (QPoint screen_pos, QTransform transform) const 
{
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect element_rect = get_element_rect (transform);
  if (element_rect.contains (screen_pos))
    return 0;

  return geom::distance (center, screen_pos);
}

void path_anchor_handle::set_mouse_hovered (bool hovered)
{
  if (m_is_highlighted == hovered)
    return;

  m_is_highlighted = hovered;
  m_editor->update ();
}

bool path_anchor_handle::start_drag (QPointF local_pos)
{
  m_edit_operation.reset (new path_edit_operation (m_item));
  m_drag_start = local_pos;
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

void path_anchor_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  anchor_handle_renderer (get_handle_center (), node_type (), m_is_highlighted).draw (canvas, state, config);
}

QPointF path_anchor_handle::get_handle_center () const
{
  if (m_drag_start != m_drag_cur)
    return m_drag_cur;

  QTransform transform = m_item->full_transform ();
  QPointF point = m_path_it.anchor_point ();
  return transform.map (point);
}

QRect path_anchor_handle::get_element_rect (QTransform transform) const
{
  int mouse_size_px = anchor_handle_renderer::get_anchor_size_px ();
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect rect (0, 0, mouse_size_px, mouse_size_px);
  rect.moveCenter (center);
  return rect;
}

void path_anchor_handle::apply_drag ()
{
  move_point ();
  m_editor->begin_changes ();
  m_edit_operation.reset ();
  m_editor->end_changes ();
}

void path_anchor_handle::move_point ()
{
  m_edit_operation->move_anchor (m_drag_cur, m_path_it);
}

std::string path_anchor_handle::item_name () const
{
  return m_item->name ();
}

const svg_path *path_anchor_handle::get_path () const
{
  auto path_data = m_item->get_computed_attribute<svg_attribute_path_data> ();
  return path_data->path ();
}


node_type_t path_anchor_handle::node_type () const
{
  auto nodetypes = m_item->get_computed_attribute<svg_attribute_nodetypes> ();
  return nodetypes->node_type ((int)m_path_it.point_index ());
}
