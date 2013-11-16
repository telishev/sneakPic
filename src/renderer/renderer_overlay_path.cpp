#include "renderer/renderer_overlay_path.h"

#include "skia/skia_includes.h"

#include <QColor>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"



renderer_overlay_path::renderer_overlay_path ()
{

}

renderer_overlay_path::~renderer_overlay_path ()
{

}

void renderer_overlay_path::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  SkPath path = qt2skia::path (m_path);
  canvas.save ();
  canvas.setMatrix (qt2skia::matrix (state.transform ()));

  SkPaint paint;
  paint.setColor (qt2skia::color (QColor ("slateblue")));
  double stroke_width = 0.0;
  paint.setStrokeWidth (stroke_width);
  paint.setStyle (SkPaint::kStroke_Style);
  paint.setAntiAlias (true);
  canvas.drawPath (path, paint);

  canvas.restore ();
}

void renderer_overlay_path::set_painter_path (const QPainterPath &path)
{
  m_path = path;
}
