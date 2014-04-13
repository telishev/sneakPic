#include "path_preview_handle.h"

#include <QPoint>
#include <QPointF>
#include <QTransform>
#include <QColor>

#include "path/svg_path_geom.h"

#include "svg/items/svg_item_path.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "renderer/path_preview_renderer.h"
#include "path/path_nearest_point.h"
#include "operations/path_edit_operation.h"
#include "path/svg_path.h"
#include "path_handles_editor.h"


path_preview_handle::path_preview_handle (path_handles_editor *editor, svg_item_path *item)
{
  m_editor = editor;
  m_item = item;
}

path_preview_handle::~path_preview_handle ()
{

}

int path_preview_handle::distance_to_mouse (QPoint screen_pos, QTransform transform) const 
{
  double max_distance = 20;
  QRectF bbox = transform.mapRect (m_item->bbox ()).adjusted (-max_distance, -max_distance, max_distance, max_distance);
  if (!bbox.contains (screen_pos))
    return 1000;

  auto path_data = m_item->get_computed_attribute<svg_attribute_path_data> ();
  path_nearest_point nearest_calc;
  return nearest_calc.get_nearest_point (screen_pos, m_item->full_transform () * transform, path_data->path ());
}

void path_preview_handle::set_mouse_hovered (bool /*hovered*/)
{

}

bool path_preview_handle::start_drag (QPointF local_pos, QTransform transform)
{
  path_nearest_point nearest_calc;
  put_in (m_edit_operation, m_item);

  m_drag_it.reset (new svg_path_geom_iterator);
  nearest_calc.get_nearest_point (transform.map (local_pos), m_item->full_transform () * transform,
                                  m_edit_operation->get_svg_path ()->get_geom (), &m_drag_t, m_drag_it.get ());

  if (m_drag_t < 0)
    {
      m_drag_it.reset ();
      m_edit_operation.reset ();
      return false;
    }

  m_prev_drag = m_drag_start = local_pos;
  return true;
}

bool path_preview_handle::drag (QPointF local_pos, QTransform /*transform*/)
{
  drag_point (local_pos);
  return true;
}

bool path_preview_handle::end_drag (QPointF local_pos, QTransform /*transform*/)
{
  drag_point (local_pos);
  m_editor->begin_changes ();
  m_edit_operation.reset ();
  m_drag_it.reset ();
  m_drag_t = -1.0;
  m_editor->end_changes ("Drag Curve");
  return true;
}

void path_preview_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  QTransform transform = m_item->full_transform ();
  auto path_data = m_item->get_computed_attribute<svg_attribute_path_data> ();
  svg_path_geom path = *path_data->path ();
  path.apply_transform (transform);

  path_preview_renderer renderer (&path, QColor ("slateblue"));
  return renderer.draw (canvas, state, config);
}

static inline double cubed (double x) { return x * x * x; }

void path_preview_handle::drag_point (QPointF local_pos)
{
  QTransform full_trans = m_item->full_transform ();
  QTransform full_inv = full_trans.inverted ();
  QPointF pos = full_inv.map (local_pos);
  QPointF init_pos = full_inv.map (m_prev_drag);
  m_prev_drag = local_pos;
  // Magic Bezier Drag Equations follow!
  // "weight" describes how the influence of the drag should be distributed
  // among the handles; 0 = front handle only, 1 = back handle only.
  double weight, t = m_drag_t;
  if (t <= 1.0 / 6.0) weight = 0;
  else if (t <= 0.5) weight = (cubed ((6 * t - 1) / 2.0)) / 2;
  else if (t <= 5.0 / 6.0) weight = (1 - cubed ((6 * (1-t) - 1) / 2.0)) / 2 + 0.5;
  else weight = 1;

  QPointF delta = pos - init_pos;
  QPointF offset0 = ((1-weight) / (3*t*(1-t)*(1-t))) * delta;
  QPointF offset1 = (weight/(3*t*t*(1-t))) * delta;
  svg_path *path = m_edit_operation->get_svg_path ();
  path->move_control_point (full_trans.map (m_drag_it->control_point (false) + offset0), *m_drag_it, false);
  path->move_control_point (full_trans.map (m_drag_it->right ().control_point (true) + offset1), m_drag_it->right (), true);
}

void path_preview_handle::interrupt_drag ()
{
  drag_point (m_drag_start);
  m_edit_operation.reset ();
  m_drag_it.reset ();
  m_editor->update ();
}
