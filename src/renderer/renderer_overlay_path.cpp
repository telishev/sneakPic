#include "renderer/renderer_overlay_path.h"

#include "skia/skia_includes.h"

#include <QColor>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"



renderer_overlay_path::renderer_overlay_path ()
{
  m_color = QColor ("slateblue");
  m_xfer_mode = SkXfermode::Mode::kSrcOver_Mode;
  m_visible = true;
}

renderer_overlay_path::~renderer_overlay_path ()
{

}

void renderer_overlay_path::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const
{
  if (!m_visible)
    return;

  SkPath path = qt2skia::path (m_path);
  canvas.save ();
  canvas.setMatrix (qt2skia::matrix (state.transform ()));

  SkPaint paint;
  paint.setColor (qt2skia::color (m_color));
  double stroke_width = 0.0;
  paint.setStrokeWidth (stroke_width);
  paint.setStyle (SkPaint::kStroke_Style);
  paint.setAntiAlias (true);
  paint.setXfermodeMode ((SkXfermode::Mode) m_xfer_mode);
  canvas.drawPath (path, paint);

  canvas.restore ();
}

void renderer_overlay_path::set_painter_path (const QPainterPath &path)
{
  m_path = path;
}

void renderer_overlay_path::set_color (const QColor &color)
{
  m_color = color;
}

void renderer_overlay_path::set_xfer_mode (int mode) // this int here is actually SkXfermode::Mode
{
  m_xfer_mode = mode;
}
