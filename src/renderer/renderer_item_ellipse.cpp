#include "renderer/renderer_item_ellipse.h"

#include <QPainter>


renderer_item_ellipse::renderer_item_ellipse ()
{
}

renderer_item_ellipse::~renderer_item_ellipse ()
{

}

void renderer_item_ellipse::draw (QPainter &painter) const
{
  configure_painter (painter);
  painter.drawEllipse (QPointF (m_cx, m_cy), m_rx, m_ry);
}

void renderer_item_ellipse::set_ellipse (double cx, double cy, double rx, double ry)
{
  m_cx = cx;
  m_cy = cy;
  m_rx = rx;
  m_ry = ry;

  m_bbox = QRectF (cx - rx, cy - ry, 2 * rx, 2 * ry);

  adjust_bbox (m_bbox);
}



