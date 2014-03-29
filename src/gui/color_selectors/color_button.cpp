#include "color_button.h"

#include "renderer/paint_server_indicator_renderer.h"


color_button::color_button (QWidget *parent /*= 0*/)
: QToolButton (parent)
{
  setAutoRaise (true);
}

color_button::~color_button ()
{

}

void color_button::set_paint_server (const renderer_paint_server *server)
{
  setIcon (create_icon (server));
}

QIcon color_button::create_icon (const renderer_paint_server *server)
{
  paint_server_indicator_renderer renderer;
  return QIcon (QPixmap::fromImage (renderer.create_image (server, iconSize ())));
}
