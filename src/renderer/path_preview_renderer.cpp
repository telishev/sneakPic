#include "renderer/path_preview_renderer.h"

#include "path/svg_path.h"

#include "skia/skia_includes.h"

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"


path_preview_renderer::path_preview_renderer (const svg_path *path, QColor color)
{
  m_path = path;
  m_color = color;
}

path_preview_renderer::~path_preview_renderer ()
{

}

void path_preview_renderer::set_path (const svg_path *path)
{
  m_path = path;
}

void path_preview_renderer::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  if (!m_path)
    return;

  canvas.save ();
  canvas.resetMatrix ();

  svg_path path = *m_path;
  path.apply_transform (state.transform ());

  SkPaint stroke_paint;
  stroke_paint.setStrokeWidth (0.0);
  stroke_paint.setStyle (SkPaint::kStroke_Style);
  stroke_paint.setAntiAlias (false);
  stroke_paint.setColor (qt2skia::color (m_color));
  canvas.drawPath (qt2skia::path (path), stroke_paint);

  canvas.restore ();
}

void path_preview_renderer::set_color (QColor color)
{
  m_color = color;
}
