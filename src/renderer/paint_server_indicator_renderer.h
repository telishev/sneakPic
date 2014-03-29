#ifndef PAINT_SERVER_INDICATOR_RENDERER_H
#define PAINT_SERVER_INDICATOR_RENDERER_H

class QImage;
class renderer_paint_server;

class paint_server_indicator_renderer
{
  bool m_is_stroke;
  double m_stroke_width;
public:
  paint_server_indicator_renderer (bool is_stroke = false, double stroke_width = 0.0);
  ~paint_server_indicator_renderer ();

  QImage create_image (const renderer_paint_server *server, QSize size);
};

#endif // PAINT_SERVER_INDICATOR_RENDERER_H
