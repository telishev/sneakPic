#ifndef RENDERER_PAINT_SERVER_H
#define RENDERER_PAINT_SERVER_H

#include <QColor>

class SkPaint;

class renderer_paint_server
{
public:
  virtual ~renderer_paint_server () {}

  virtual void set_opacity (double opacity) = 0;
  virtual void fill_paint (SkPaint &paint) const = 0;

  virtual renderer_paint_server *clone () const = 0;
  virtual bool need_to_render () const { return true; }
};

class renderer_painter_server_color : public renderer_paint_server
{
  QColor m_color;
public:
  renderer_painter_server_color (const QColor &color);
  virtual ~renderer_painter_server_color () {}

  virtual void set_opacity (double opacity) override;
  virtual void fill_paint (SkPaint &paint) const override;

  virtual renderer_paint_server *clone () const override;
};

class renderer_painter_server_none : public renderer_paint_server
{
public:

  virtual void set_opacity (double /*opacity*/) override {}
  virtual void fill_paint (SkPaint &paint) const override;
  virtual renderer_paint_server *clone () const override;
  virtual bool need_to_render () const override { return false; }
};


#endif // RENDERER_PAINT_SERVER_H
