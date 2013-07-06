#include "renderer/renderer_base_shape_item.h"

#include <QPainter>
#include "renderer_paint_server.h"

renderer_base_shape_item::renderer_base_shape_item ()
{
  m_opacity = 1.0;
  m_has_clip_path = false;
  visible = true;
}

renderer_base_shape_item::~renderer_base_shape_item ()
{
}

void renderer_base_shape_item::set_stroke_linecap (Qt::PenCapStyle linecap)
{
  m_pen.setCapStyle (linecap);
}

void renderer_base_shape_item::set_stroke_linejoin (Qt::PenJoinStyle linejoin)
{
  m_pen.setJoinStyle (linejoin);
}

void renderer_base_shape_item::set_stroke_visibility (bool visible_arg)
{
  visible = visible_arg;
}

void renderer_base_shape_item::set_stroke_miterlimit (double miterlimit)
{
  m_pen.setMiterLimit (miterlimit);
}

void renderer_base_shape_item::set_stroke_width (double width)
{
  m_pen.setWidthF (width);
}

bool renderer_base_shape_item::configure_painter (QPainter &painter) const
{
  if (!visible)
    return false;

  painter.setPen (m_pen);
  painter.setBrush (m_brush);
  painter.setOpacity (m_opacity);
  if (m_has_clip_path)
    painter.setClipPath (m_clip_path);
  else
    painter.setClipping (false);
  return true;
}

void renderer_base_shape_item::adjust_bbox (QRectF &bbox, const QTransform &full_transform) const
{
  /// add pen width to a bbox
  double adjust_value = m_pen.widthF ();
  bbox.adjust (-adjust_value, -adjust_value, adjust_value, adjust_value);
  bbox = full_transform.mapRect (bbox);
}

void renderer_base_shape_item::set_stroke_server (const renderer_paint_server *server)
{
  m_pen.setBrush (server->get_brush ());
}

void renderer_base_shape_item::set_fill_server (const renderer_paint_server *server)
{
  m_brush = server->get_brush ();
}

void renderer_base_shape_item::set_opacity (double opacity)
{
  m_opacity = opacity;
}
