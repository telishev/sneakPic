#include "color_indicator.h"

#include <QPainter>

#include "gui/paint_helper.h"

static const int COLOR_INDICATOR_DEFAULT_SIZE = 40;
static const int STROKE_WIDTH_HALF = 10;

color_indicator::color_indicator (bool is_stroke, QWidget *parent)
  : QWidget (parent)
{
  m_is_stroke = is_stroke;
  setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
}

color_indicator::~color_indicator ()
{

}

QSize color_indicator::sizeHint () const
{
  return QSize (COLOR_INDICATOR_DEFAULT_SIZE, COLOR_INDICATOR_DEFAULT_SIZE);
}

void color_indicator::paintEvent (QPaintEvent * /*event*/)
{
  QPainter painter (this);
  paint_helper ().draw_checkerboard (painter, width (), height ());

  
  if (m_is_stroke)
    {
      QPen pen (m_server.color ());
      pen.setWidth (STROKE_WIDTH_HALF * 2);
      painter.setPen (pen);
      painter.setBrush (Qt::NoBrush);
    }
  else
    {
      painter.setPen (Qt::black);
      painter.setBrush (m_server.color ());
    }
  
  painter.drawRect (draw_rect ());
  paint_helper ().draw_border (painter, width (), height ());
}

void color_indicator::set_paint_server (const item_paint_server &server)
{
  m_server = server;
  update ();
}

QRect color_indicator::draw_rect () const
{
  QRect cur_rect = rect ();
  if (!m_is_stroke)
    return cur_rect;

  cur_rect.adjusted (STROKE_WIDTH_HALF, STROKE_WIDTH_HALF, -STROKE_WIDTH_HALF, -STROKE_WIDTH_HALF);
  return cur_rect;
}

void color_indicator::mouseReleaseEvent (QMouseEvent * /*event*/)
{
  emit clicked ();
}
