#include "renderer/renderer_paint_server.h"

#include "renderer/qt2skia.h"

#include "skia/skia_includes.h"

renderer_painter_server_color::renderer_painter_server_color (const QColor &color)
{
  m_color = color;
}

void renderer_painter_server_color::set_opacity (double opacity)
{
  m_color.setAlphaF (opacity);
}

void renderer_painter_server_color::fill_paint (SkPaint &paint, QRectF ) const 
{
  paint.setColor (qt2skia::color (m_color));
}

renderer_painter_server_color *renderer_painter_server_color::clone () const 
{
  return new renderer_painter_server_color (m_color);
}

void renderer_painter_server_none::fill_paint (SkPaint &paint, QRectF) const 
{
  paint.setColor (0);
}

renderer_painter_server_none *renderer_painter_server_none::clone () const 
{
  return new renderer_painter_server_none;
}
