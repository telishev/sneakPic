#include "color_indicator.h"

#include <QPainter>

#include "gui/paint_helper.h"
#include "renderer/renderer_paint_server.h"
#include "renderer/paint_server_indicator_renderer.h"


static const int COLOR_INDICATOR_DEFAULT_SIZE = 40;
static const int STROKE_WIDTH_HALF = 10;

color_indicator::color_indicator (bool is_stroke, QWidget *parent)
  : QWidget (parent)
{
  m_is_stroke = is_stroke;
  setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_server.reset (new renderer_painter_server_color (Qt::black));
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

  draw_rect (painter);
  paint_helper ().draw_border (painter, width (), height ());
}

void color_indicator::set_paint_server (const renderer_paint_server *server)
{
  m_server.reset (server->clone ());
  update ();
}

QRect color_indicator::get_draw_rect () const
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

void color_indicator::draw_rect (QPainter &painter)
{
  paint_server_indicator_renderer renderer (m_is_stroke, STROKE_WIDTH_HALF * 2);
  painter.drawImage (get_draw_rect (), renderer.create_image (m_server.get (), {get_draw_rect ().width (), get_draw_rect ().height ()}));
}
