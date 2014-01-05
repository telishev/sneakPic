#include "color_indicator.h"

#include <QPainter>

static int COLOR_INDICATOR_DEFAULT_SIZE = 25;

QSize color_indicator::sizeHint () const
{
  return QSize (COLOR_INDICATOR_DEFAULT_SIZE, COLOR_INDICATOR_DEFAULT_SIZE);
}

void color_indicator::paintEvent (QPaintEvent * /*event*/)
{
  QPainter painter (this);
  draw_checkerboard (painter);
  painter.setBrush (QBrush (m_color ? *m_color : placeholder_color));
  painter.drawRect (this->rect ());
  draw_border (painter);
}

color_indicator::color_indicator (QWidget *parent, QColor *color)
  : color_selector (parent, color)
{
  setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
}
