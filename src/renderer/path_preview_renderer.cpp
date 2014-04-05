#include "renderer/path_preview_renderer.h"

#include "path/svg_path_geom.h"

#include "skia/skia_includes.h"

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"


path_preview_renderer::path_preview_renderer (const svg_path_geom *path, QColor color)
: simple_path_renderer ({}, color)
{
  m_path = path;
}

path_preview_renderer::~path_preview_renderer ()
{

}

void path_preview_renderer::set_path (const svg_path_geom *path)
{
  m_path = path;
}

SkPath path_preview_renderer::get_path () const
{
  return m_path ? qt2skia::path (*m_path) : SkPath ();
}

simple_path_renderer::simple_path_renderer (SkPath path, QColor color)
{
  m_path = path;
  m_color = color;
}

simple_path_renderer::~simple_path_renderer ()
{

}

void simple_path_renderer::set_simple_path (SkPath path)
{
  m_path = path;
}

void simple_path_renderer::set_color (QColor color)
{
  m_color = color;
}

void simple_path_renderer::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const
{
  canvas.save ();
  canvas.setMatrix (qt2skia::matrix (state.transform ()));

  SkPaint stroke_paint;
  stroke_paint.setStrokeWidth (0.0);
  stroke_paint.setStyle (SkPaint::kStroke_Style);
  stroke_paint.setAntiAlias (false);
  stroke_paint.setColor (qt2skia::color (m_color));
  canvas.drawPath (get_path (), stroke_paint);

  canvas.restore ();
}

SkPath simple_path_renderer::get_path () const
{
  return m_path;
}
