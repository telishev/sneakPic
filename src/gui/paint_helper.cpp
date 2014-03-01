#include "paint_helper.h"

#include <QPainter>

static const int CHECKERBOARD_ELEMENT_SIZE = 5;


void paint_helper::draw_checkerboard (QPainter &painter, double width, double height)
{
  painter.setPen (Qt::NoPen);
  for (int i = 0; i < width / CHECKERBOARD_ELEMENT_SIZE + 1; i++)
    for (int j = 0; j < height / CHECKERBOARD_ELEMENT_SIZE + 1; j++)
      painter.fillRect (QRect (i * CHECKERBOARD_ELEMENT_SIZE + BORDER_WIDTH, j * CHECKERBOARD_ELEMENT_SIZE + BORDER_WIDTH,
                               CHECKERBOARD_ELEMENT_SIZE, CHECKERBOARD_ELEMENT_SIZE), (i + j) % 2 == 0 ? QBrush ("White") : QBrush ("LightGrey"));
}

void paint_helper::draw_border (QPainter &painter, double width, double height)
{
  painter.setPen (QPen (QBrush ("Grey"), BORDER_WIDTH, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
  painter.drawRect (BORDER_WIDTH / 2, BORDER_WIDTH / 2, width - BORDER_WIDTH, height - BORDER_WIDTH);
}