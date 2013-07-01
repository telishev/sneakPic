#include "renderer/renderer_item_rect.h"

#include <QPainter>


renderer_item_rect::renderer_item_rect ()
{
}

renderer_item_rect::~renderer_item_rect ()
{

}

void renderer_item_rect::draw (QPainter &painter) const
{
  configure_painter (painter);
  painter.drawRoundedRect (m_rect, m_rx, m_ry);
}

void renderer_item_rect::set_rect (double x, double y, double width, double height, double rx, double ry)
{
  m_rect = QRectF (x, y, width, height);
  m_rx = rx;
  m_ry = ry;
  m_bbox = m_rect;

  adjust_bbox (m_bbox);
}

