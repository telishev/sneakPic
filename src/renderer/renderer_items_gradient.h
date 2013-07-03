#ifndef RENDERER_ITEMS_GRADIENT_H
#define RENDERER_ITEMS_GRADIENT_H

#include "renderer/renderer_paint_server.h"

#include <QLinearGradient>
#include <QRadialGradient>

enum class spread_method;
enum class gradient_units;

class renderer_base_gradient_item : public renderer_paint_server
{
protected:
  QGradient m_gradient;
public:

  virtual void set_opacity (double opacity) override;
  virtual QBrush get_brush () const override;

  void add_stop (double position, const QColor &color);
  void set_spread (spread_method spread);
  void set_gradient_units (gradient_units units);
};

class renderer_linear_gradient : public renderer_base_gradient_item
{
public:
  renderer_linear_gradient (double x1, double y1, double x2, double y2)
  {
    m_gradient = QLinearGradient (x1, y1, x2, y2);
  }
};

class renderer_radial_gradient : public renderer_base_gradient_item
{
public:
  renderer_radial_gradient (double cx, double cy, double r, double fx, double fy)
  {
    m_gradient = QRadialGradient (cx, cy, r, fx, fy);
  }
};
#endif // RENDERER_ITEMS_GRADIENT_H
