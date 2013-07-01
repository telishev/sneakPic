#include "renderer/renderer_item_line.h"

#include <QPainter>


renderer_item_line::renderer_item_line ()
{
}

renderer_item_line::~renderer_item_line ()
{

}

void renderer_item_line::draw (QPainter &painter) const
{
  configure_painter (painter);
  painter.drawLine (m_line);
}

void renderer_item_line::set_line (double x1, double y1, double x2, double y2)
{
  m_line = QLineF (x1, y1, x2, y2);
  m_bbox = QRectF (qMin (x1, x2), qMin (y1, y2), fabs (x1 - x2), fabs (y1 - y2));

  adjust_bbox (m_bbox);
}

