#include "renderer/renderer_paint_server.h"

renderer_painter_server_color::renderer_painter_server_color (const QColor &color)
{
  m_brush = QBrush (color);
}

void renderer_painter_server_color::set_opacity (double opacity)
{
  QColor color = m_brush.color ();
  color.setAlphaF (opacity);
  m_brush.setColor (color);
}

QBrush renderer_painter_server_color::get_brush () const 
{
  return m_brush;
}
