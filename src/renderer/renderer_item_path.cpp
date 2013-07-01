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
  configure_painter (painter);
  painter.drawPath (m_path);
}

void renderer_item_path::set_painter_path (const QPainterPath &path)
{
  m_path = path;
  m_bbox = m_path.controlPointRect ();
  /// add pen width to a bbox
  double adjust_value = m_pen.widthF ();
  m_bbox.adjust (-adjust_value, -adjust_value, adjust_value, adjust_value);
}
