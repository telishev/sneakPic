#include "renderer/renderer_item_path.h"

#include <QPainter>


renderer_item_path::renderer_item_path ()
{
}

renderer_item_path::~renderer_item_path ()
{

}

void renderer_item_path::draw (QPainter &painter) const
{
  painter.setPen (m_pen);
  painter.setBrush (m_brush);
  painter.drawPath (m_path);
}

void renderer_item_path::set_stroke_width (double width)
{
  m_pen.setWidthF (width);
}

void renderer_item_path::set_stroke_color (const QColor &color)
{
  m_pen.setColor (color);
}

void renderer_item_path::set_show_stroke (bool show)
{
  m_pen.setStyle (show ? Qt::SolidLine : Qt::NoPen);
}

void renderer_item_path::set_fill_color (const QColor &color)
{
  m_brush.setColor (color);
}

void renderer_item_path::set_show_fill (bool show)
{
  m_brush.setStyle (show ? Qt::SolidPattern : Qt::NoBrush);
}

void renderer_item_path::set_painter_path (const QPainterPath &path)
{
  m_path = path;
}
