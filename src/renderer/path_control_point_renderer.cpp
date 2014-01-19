#include "renderer/path_control_point_renderer.h"

#include <QColor>

#include "skia/skia_includes.h"

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"

#include "path/geom_helpers.h"

path_control_point_renderer::path_control_point_renderer (QPointF anchor, QPointF control_point, bool is_highlighted)
{
  m_is_visible = true;
  m_anchor = anchor;
  m_control_point = control_point;
  m_is_highlighted = is_highlighted;
}

path_control_point_renderer::path_control_point_renderer ()
{
  m_is_visible = false;
  m_anchor = QPointF ();
  m_control_point = QPointF ();
  m_is_highlighted = false;
}

path_control_point_renderer::~path_control_point_renderer ()
{

}

void path_control_point_renderer::set_anchor (QPointF anchor)
{
  m_anchor = anchor;
}

void path_control_point_renderer::set_control_point (QPointF control_point)
{
  m_control_point = control_point;
}

void path_control_point_renderer::set_visible (bool is_visible)
{
  m_is_visible = is_visible;
}

void path_control_point_renderer::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  if (!m_is_visible)
    return;

  canvas.save ();
  canvas.resetMatrix ();

  QPoint center = geom::nearest_point (state.transform ().map (m_control_point));
  QPoint anchor_center = geom::nearest_point (state.transform ().map (m_anchor));
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

QColor path_control_point_renderer::current_color () const
{
  if (m_is_highlighted)
    return QColor ("lightcoral");
  else
    return Qt::transparent;
}

QColor path_control_point_renderer::line_color () const
{
  return QColor ("lightblue");
}

void path_control_point_renderer::set_highlighted (bool is_highlighted)
{
  m_is_highlighted = is_highlighted;
}
