#include "renderer/anchor_handle_renderer.h"

#include <QColor>

#include "skia/skia_includes.h"

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"

#include "path/svg_path_geom.h"
#include "svg/attributes/svg_attribute_nodetypes.h"


anchor_handle_renderer::anchor_handle_renderer (QPointF pos, handle_type node_type, bool is_highlighted)
{
  m_node_type = node_type;
  m_pos = pos;
  m_is_highlighted = is_highlighted;
  m_is_visible = true;
  m_is_selected = false;
}

anchor_handle_renderer::anchor_handle_renderer ()
{
  m_node_type = handle_type::SQUARE;
  m_pos = QPointF ();
  m_is_highlighted = false;
  m_is_visible = false;
}

anchor_handle_renderer::~anchor_handle_renderer ()
{

}


void anchor_handle_renderer::set_pos (QPointF pos)
{
  m_pos = pos;
}

void anchor_handle_renderer::set_node_type (handle_type node_type)
{
  m_node_type = node_type;
}


void anchor_handle_renderer::set_highlighted (bool is_highlighted)
{
  m_is_highlighted = is_highlighted;
}

void anchor_handle_renderer::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  if (!m_is_visible)
    return;

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

void anchor_handle_renderer::draw_anchor (SkCanvas &canvas, const SkRect &rect, SkPaint &paint) const
{
  switch (m_node_type)
  {
    case handle_type::DIAMOND:
    {
      canvas.save ();
      canvas.translate (rect.centerX (), rect.centerY ());
      canvas.rotate (45);
      SkRect moved_rect = rect;
      moved_rect.offset (-rect.centerX (), -rect.centerY ());
      paint.setAntiAlias (true);
      canvas.drawRect (moved_rect, paint);
      canvas.restore ();
      break;
    }
    case handle_type::SQUARE:
      canvas.drawRect (rect, paint);
      break;
    case handle_type::CIRCLE:
      canvas.drawOval (rect, paint);
  }
}

QRect anchor_handle_renderer::get_element_rect (QTransform transform) const
{
  QPoint center = geom::nearest_point (transform.map (m_pos));
  QRect rect (0, 0, anchor_size_px, anchor_size_px);
  rect.moveCenter (center);
  return rect;
}

QColor anchor_handle_renderer::current_color () const
{
  if (m_is_highlighted)
    return QColor ("lightcoral");
  else if (m_is_selected)
    return QColor ("blue");
  else
    return QColor ("gray");
}

void anchor_handle_renderer::set_visible (bool visible)
{
  m_is_visible = visible;
}

void anchor_handle_renderer::set_is_selected (bool is_selected)
{
  m_is_selected = is_selected;
}
