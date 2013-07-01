#include "renderer/renderer_base_shape_item.h"


#include <QPainter>


renderer_base_shape_item::renderer_base_shape_item ()
{
}

renderer_base_shape_item::~renderer_base_shape_item ()
{

}

void renderer_base_shape_item::set_stroke_width (double width)
{
  m_pen.setWidthF (width);
}

void renderer_base_shape_item::set_stroke_color (const QColor &color)
{
  m_pen.setColor (color);
}

void renderer_base_shape_item::set_show_stroke (bool show)
{
  m_pen.setStyle (show ? Qt::SolidLine : Qt::NoPen);
}

void renderer_base_shape_item::set_fill_color (const QColor &color)
{
  m_brush.setColor (color);
}

void renderer_base_shape_item::set_show_fill (bool show)
{
  m_brush.setStyle (show ? Qt::SolidPattern : Qt::NoBrush);
}

void renderer_base_shape_item::configure_painter (QPainter &painter) const
{
  painter.setPen (m_pen);
  painter.setBrush (m_brush);
}


