#include "renderer/renderer_item_path.h"

#include <QPainter>

#include "renderer/renderer_state.h"


renderer_item_path::renderer_item_path ()
{
}

renderer_item_path::~renderer_item_path ()
{

}

void renderer_item_path::draw (QPainter &painter, const renderer_state &state) const
{
  QTransform item_transform = transform () * state.transform ();
  QRectF transformed_rect = state.transform ().mapRect (bounding_box ());
  if (!state.rect ().intersects (transformed_rect.toRect ()))
    return;

  painter.setTransform (item_transform);
  if (!configure_painter (painter))
    return;

  painter.drawPath (m_path);
  painter.setClipping (false);
}

void renderer_item_path::set_painter_path (const QPainterPath &path, const QTransform &full_transform)
{
  m_path = path;
  m_bbox = m_path.controlPointRect ();
  adjust_bbox (m_bbox, full_transform);
}
