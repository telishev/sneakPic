#ifndef RENDERER_ITEMS_GRADIENT_H
#define RENDERER_ITEMS_GRADIENT_H

#include "renderer/renderer_paint_server.h"

#include <utility>
#include <vector>
#include <QTransform>

class QColor;

enum class spread_method;
enum class gradient_units;

class renderer_base_gradient_item : public renderer_paint_server
{
protected:
  std::vector<std::pair<double, QColor>> m_stops;
  spread_method m_spread;
  gradient_units m_units;
  QTransform m_transform;
public:

  virtual void set_opacity (double opacity) override;

  void add_stop (double position, const QColor &color) { m_stops.push_back (std::make_pair (position, color)); }
  void set_spread (spread_method spread) { m_spread = spread; }
  void set_gradient_units (gradient_units units) { m_units = units; }
  void set_transform (const QTransform &transform) { m_transform = transform; }
};

class renderer_linear_gradient : public renderer_base_gradient_item
{
  double m_x1, m_y1, m_x2, m_y2;
public:
  renderer_linear_gradient (double x1, double y1, double x2, double y2) 
  {
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
  }

  virtual void fill_paint (SkPaint &paint) const override;
  virtual renderer_paint_server *clone () const override;
};

class renderer_radial_gradient : public renderer_base_gradient_item
{
  double m_cx, m_cy, m_r, m_fx, m_fy;
public:
  renderer_radial_gradient (double cx, double cy, double r, double fx, double fy)
  {
    m_cx = cx;
    m_cy = cy;
    m_r = r;
    m_fx = fx;
    m_fy = fy;
  }

  virtual void fill_paint (SkPaint &paint) const override;
  virtual renderer_paint_server *clone () const override;
};
#endif // RENDERER_ITEMS_GRADIENT_H
