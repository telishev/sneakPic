#ifndef RENDERER_PAINT_SERVER_H
#define RENDERER_PAINT_SERVER_H

#include <QBrush>

class renderer_paint_server
{
public:
  virtual ~renderer_paint_server () {}

  virtual void set_opacity (double opacity) = 0;
  virtual QBrush get_brush () const = 0;
};

class renderer_painter_server_color : public renderer_paint_server
{
  QBrush m_brush;
public:
  renderer_painter_server_color (const QColor &color);
  virtual ~renderer_painter_server_color () {}

  virtual void set_opacity (double opacity) override;
  virtual QBrush get_brush () const override;
};

class renderer_painter_server_none : public renderer_paint_server
{
public:

  virtual void set_opacity (double /*opacity*/) override {}
  virtual QBrush get_brush () const override { return QBrush (); }
};


#endif // RENDERER_PAINT_SERVER_H
