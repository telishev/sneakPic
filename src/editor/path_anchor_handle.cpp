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


static const int mouse_size_px = 7;


path_anchor_handle::path_anchor_handle (path_handles_editor *editor, svg_item_path *item, int anchor_id)
{
  m_item = item;
  m_anchor_id = anchor_id;
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

void path_anchor_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  canvas.save ();
  canvas.resetMatrix ();

  QRect element_rect = get_element_rect (state.transform ());
  SkRect rect = qt2skia::rect (element_rect);

  SkPaint fill_paint;
  fill_paint.setStyle (SkPaint::kFill_Style);
  fill_paint.setColor (qt2skia::color (current_color ()));
  draw_anchor (canvas, rect, fill_paint);

  SkPaint stroke_paint;
  stroke_paint.setStrokeWidth (0.0);
  stroke_paint.setStyle (SkPaint::kStroke_Style);
  stroke_paint.setColor (SK_ColorBLACK);
  draw_anchor (canvas, rect, stroke_paint);

  canvas.restore ();
}

QPointF path_anchor_handle::get_handle_center () const
{
  if (m_drag_start != m_drag_cur)
    return m_drag_cur;

  QTransform transform = m_item->full_transform ();
  QPointF point = get_path ()->point (m_anchor_id);
  return transform.map (point);
}

QRect path_anchor_handle::get_element_rect (QTransform transform) const
{
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
  m_edit_operation->move_anchor (m_drag_cur, m_anchor_id);
}

QColor path_anchor_handle::current_color () const
{
  if (m_is_highlighted)
    return QColor ("lightcoral");
  else
    return QColor ("gray");
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

void path_anchor_handle::draw_anchor (SkCanvas &canvas, const SkRect &rect, SkPaint &paint) const
{
  if (is_cusp_node ())
    {
      canvas.save ();
      canvas.translate (rect.centerX (), rect.centerY ());
      canvas.rotate (45);
      SkRect moved_rect = rect;
      moved_rect.offset (-rect.centerX (), -rect.centerY ());
      paint.setAntiAlias (true);
      canvas.drawRect (moved_rect, paint);
      canvas.restore ();
    }
  else
    canvas.drawRect (rect, paint);
}

bool path_anchor_handle::is_cusp_node () const
{
  auto nodetypes = m_item->get_computed_attribute<svg_attribute_nodetypes> ();
  return nodetypes->node_type (m_anchor_id) == node_type_t::CUSP;
}
