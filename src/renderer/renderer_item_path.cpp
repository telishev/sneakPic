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
  if (!configure_painter (painter))
    return;

  painter.drawPath (m_path);
}

void renderer_item_path::set_painter_path (const QPainterPath &path)
{
  m_path = path;
  m_bbox = m_path.controlPointRect ();
  adjust_bbox (m_bbox);
}
