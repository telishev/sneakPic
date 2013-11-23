#include "path_control_point_handle.h"

#include "editor/path_control_point_handle.h"

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
#include "operations/path_edit_operation.h"


static const int handle_radius_px = 3;


path_control_point_handle::path_control_point_handle (path_handles_editor *editor, svg_item_path *item, int control_id,
                                                      bool is_left_handle)
{
  m_left_handle = is_left_handle;
  m_item = item;
  m_control_id = control_id;
  m_editor = editor;
  m_is_highlighted = false;
}

path_control_point_handle::~path_control_point_handle ()
{

}

int path_control_point_handle::distance_to_mouse (QPoint screen_pos, QTransform transform) const 
{
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  return geom::distance (center, screen_pos);
}

void path_control_point_handle::set_mouse_hovered (bool hovered)
{
  if (m_is_highlighted == hovered)
    return;

  m_is_highlighted = hovered;
  m_editor->update ();
}

bool path_control_point_handle::start_drag (QPointF local_pos)
{
  m_drag_start = local_pos;
  m_edit_operation.reset (new path_edit_operation (m_item));
  return true;
}

bool path_control_point_handle::drag (QPointF local_pos)
{
  m_drag_cur = local_pos;
  move_point ();
  m_editor->update ();
  return true;
}

bool path_control_point_handle::end_drag (QPointF local_pos)
{
  m_drag_cur = local_pos;
  apply_drag ();
  m_drag_cur = m_drag_start = QPointF ();
  m_editor->update ();
  return true;
}

void path_control_point_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  canvas.save ();
  canvas.resetMatrix ();

  QPoint center = geom::nearest_point (state.transform ().map (get_handle_center ()));
  QPoint anchor_center = geom::nearest_point (state.transform ().map (get_anchor_center ()));
  SkPath path;
  path.addCircle (SkFloatToScalar (center.x ()), SkFloatToScalar (center.y ()), SkFloatToScalar (handle_radius_px));

  /// draw line
  SkPaint stroke_paint;
  stroke_paint.setStrokeWidth (0.0);
  stroke_paint.setStyle (SkPaint::kStroke_Style);

  stroke_paint.setColor (qt2skia::color (line_color ()));
  canvas.drawLine (anchor_center.x (), anchor_center.y (), center.x (), center.y (), stroke_paint);

  /// draw circle
  SkPaint fill_paint;
  fill_paint.setStyle (SkPaint::kFill_Style);
  fill_paint.setColor (qt2skia::color (current_color ()));
  canvas.drawPath (path, fill_paint);

  /// draw circle outline
  stroke_paint.setColor (SK_ColorBLACK);
  canvas.drawPath (path, stroke_paint);

  canvas.restore ();
}

QPointF path_control_point_handle::get_handle_center () const
{
  if (m_drag_start != m_drag_cur)
    return m_drag_cur;

  QTransform transform = m_item->full_transform ();
  QPointF point = get_path ()->control_point (m_control_id, m_left_handle);
  return transform.map (point);
}

QPointF path_control_point_handle::get_anchor_center () const
{
  QTransform transform = m_item->full_transform ();
  QPointF point = get_path ()->point (m_control_id);
  return transform.map (point);
}

void path_control_point_handle::apply_drag ()
{
  move_point ();
  m_editor->begin_changes ();
  m_edit_operation.reset ();
  m_editor->end_changes ();
}

void path_control_point_handle::move_point ()
{
  m_edit_operation->move_control_point (m_drag_cur, m_control_id, m_left_handle);;
}

QColor path_control_point_handle::current_color () const
{
  if (m_is_highlighted)
    return QColor ("lightcoral");
  else
    return Qt::transparent;
}

QColor path_control_point_handle::line_color () const
{
  return QColor ("lightblue");
}

const svg_path * path_control_point_handle::get_path () const
{
  auto path_data = m_item->get_computed_attribute<svg_attribute_path_data> ();
  return path_data->path ();
}



