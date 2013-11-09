#include "editor/path_control_point.h"

#include <QPointF>
#include <QRect>
#include <QTransform>

#pragma warning(push, 0)
#include <SkCanvas.h>
#pragma warning(pop)

#include "common/debug_utils.h"


#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"
#include "renderer/svg_painter.h"

#include "path/svg_path.h"

#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/items/svg_item_path.h"
#include "svg/svg_document.h"


static const int mouse_size_px = 7;


path_control_point::path_control_point (svg_painter *painter, svg_item_path *item, int control_point_id, svg_path *path)
{
  m_path = path;
  m_item = item;
  m_control_point_id = control_point_id;
  m_painter = painter;
  m_is_highlighted = false;
}

path_control_point::~path_control_point ()
{

}

int path_control_point::distance_to_mouse (QPoint screen_pos, QTransform transform) const 
{
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect element_rect = get_element_rect (transform);
  if (element_rect.contains (screen_pos))
    return 0;

  return geom::distance (center, screen_pos);
}

void path_control_point::set_mouse_hovered (bool hovered)
{
  if (m_is_highlighted == hovered)
    return;

  m_is_highlighted = hovered;
  m_painter->update ();
}

bool path_control_point::start_drag (QPointF local_pos)
{
  m_drag_start = local_pos;
  return true;
}

bool path_control_point::drag (QPointF local_pos)
{
  m_drag_cur = local_pos;
  move_point (m_path);
  m_painter->update ();
  return true;
}

bool path_control_point::end_drag (QPointF local_pos)
{
  m_drag_cur = local_pos;
  apply_drag ();
  m_drag_cur = m_drag_start = QPointF ();
  m_painter->update ();
  return true;
}

void path_control_point::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  canvas.save ();
  canvas.resetMatrix ();

  QRect element_rect = get_element_rect (state.transform ());
  SkRect fill_rect = qt2skia::rect (element_rect);
  SkRect stroke_rect = qt2skia::rect (element_rect);

  SkPaint fill_paint;
  fill_paint.setStyle (SkPaint::kFill_Style);
  fill_paint.setColor (qt2skia::color (current_color ()));
  canvas.drawRect (fill_rect, fill_paint);

  SkPaint stroke_paint;
  stroke_paint.setStrokeWidth (0.0);
  stroke_paint.setStyle (SkPaint::kStroke_Style);
  stroke_paint.setColor (SK_ColorBLACK);
  canvas.drawRect (stroke_rect, stroke_paint);

  canvas.restore ();
}

QPointF path_control_point::get_handle_center () const
{
  if (m_drag_start != m_drag_cur)
    return m_drag_cur;

  QTransform transform = m_item->full_transform ();
  QPointF point = m_path->point (m_control_point_id);
  return transform.map (point);
}

QRect path_control_point::get_element_rect (QTransform transform) const
{
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect rect (0, 0, mouse_size_px, mouse_size_px);
  rect.moveCenter (center);
  return rect;
}

void path_control_point::apply_drag ()
{
  {
    auto path_data = m_item->get_attribute_for_change<svg_attribute_path_data> ();
    move_point (path_data->path ());
  }

  m_painter->document ()->apply_changes ();
}

void path_control_point::move_point (svg_path *path)
{
  QTransform transform = m_item->full_transform ().inverted ();
  path->set_point (m_control_point_id, transform.map (m_drag_cur));
}

QColor path_control_point::current_color () const
{
  if (m_is_highlighted)
    return QColor ("lightcoral");
  else
    return QColor ("gray");
}
