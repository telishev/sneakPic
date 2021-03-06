#ifndef RENDERER_ITEMS_GRADIENT_H
#define RENDERER_ITEMS_GRADIENT_H

#include "renderer/renderer_paint_server.h"

#include <utility>
#include <vector>
#include <QTransform>

class QColor;
class QGradient;

class SkShader;

typedef unsigned int SkColor;
typedef float   SkScalar;

enum class spread_method;
enum class gradient_units;

class renderer_base_gradient_item : public renderer_paint_server
{
protected:
  vector<pair<double, QColor>> m_stops;
  spread_method m_spread;
  gradient_units m_units;
  QTransform m_transform;
public:
  renderer_base_gradient_item ();

  vector<pair<double, QColor>> &stops () { return m_stops; }
  const vector<pair<double, QColor>> &stops () const { return m_stops; }
  spread_method spread () const { return m_spread; }
  gradient_units units () const { return m_units; }
  QTransform transform () const { return m_transform; }

  virtual void set_opacity (double opacity) override;

  void add_stop (double position, const QColor &color) { m_stops.push_back (std::make_pair (position, color)); }
  void set_spread (spread_method spread) { m_spread = spread; }
  void set_gradient_units (gradient_units units) { m_units = units; }
  void set_transform (const QTransform &transform) { m_transform = transform; }
  virtual void fill_paint (SkPaint &paint, QRectF bbox) const override;

  void convert_to_bbox_units (QRectF bbox);

private:
  virtual SkShader *create_shader (SkColor *colors, SkScalar *pos, int size, int mode) const = 0;

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

  virtual renderer_linear_gradient *clone () const override;
  virtual renderer_paint_server_type type () const override { return renderer_paint_server_type::LINEAR_GRADIENT; }

  double x1 () const { return m_x1; }
  double x2 () const { return m_x2; }
  double y1 () const { return m_y1; }
  double y2 () const { return m_y2; }

  void set_x1 (double val) { m_x1 = val; }
  void set_y1 (double val) { m_y1 = val; }
  void set_x2 (double val) { m_x2 = val; }
  void set_y2 (double val) { m_y2 = val; }

private:
  virtual SkShader *create_shader (SkColor *colors, SkScalar *pos, int size, int mode) const override;
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

  virtual renderer_radial_gradient *clone () const override;

  virtual renderer_paint_server_type type () const override { return renderer_paint_server_type::RADIAL_GRADIENT; }

  double cx () const { return m_cx; }
  double cy () const { return m_cy; }
  double fx () const { return m_fx; }
  double fy () const { return m_fy; }
  double r () const { return m_r; }

  void set_cx (double val) { m_cx = val; }
  void set_cy (double val) { m_cy = val; }
  void set_fx (double val) { m_fx = val; }
  void set_fy (double val) { m_fy = val; }
  void set_r (double val) { m_r = val; }

private:
  virtual SkShader *create_shader (SkColor *colors, SkScalar *pos, int size, int mode) const override ;
};
#endif // RENDERER_ITEMS_GRADIENT_H
