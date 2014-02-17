#include "renderer/renderer_items_gradient.h"

#include "svg/attributes/svg_attributes_enum.h"

#include "renderer/qt2skia.h"

#include <memory>

#include "skia/skia_includes.h"

void renderer_base_gradient_item::set_opacity (double opacity)
{
  for (size_t i = 0; i < m_stops.size (); i++)
    m_stops[i].second.setAlphaF (opacity * m_stops[i].second.alphaF ());
}

void renderer_linear_gradient::fill_paint (SkPaint &paint) const
{
  SkPoint points[2] = { SkPoint::Make (SkFloatToScalar (m_x1), SkFloatToScalar (m_y1)),
                        SkPoint::Make (SkFloatToScalar (m_x2), SkFloatToScalar (m_y2)) };

  unique_ptr<SkColor []> colors (new SkColor [m_stops.size ()]);
  unique_ptr<SkScalar[]> pos (new SkScalar [m_stops.size ()]);
  for (size_t i = 0; i < m_stops.size (); i++)
    {
      colors[i] = qt2skia::color (m_stops[i].second);
      pos[i] = SkFloatToScalar (m_stops[i].first);
    }

  SkShader::TileMode mode;
  switch (m_spread)
    {
    case spread_method::PAD: mode = SkShader::kClamp_TileMode; break;
    case spread_method::REFLECT: mode = SkShader::kMirror_TileMode; break;
    case spread_method::REPEAT: mode = SkShader::kRepeat_TileMode; break;
    case spread_method::INVALID: return;
    }

  /// standard says to stop rendering in case of 0 stops
  if (m_stops.size () == 0)
    {
      paint.setColor (0);
      return;
    }
  /// and to render with solid color in case of 1 stop
  else if (m_stops.size () == 1)
    {
      paint.setColor (colors[0]);
      return;
    }

  SkShader *shader = SkGradientShader::CreateLinear (points, colors.get (), pos.get (), (int)m_stops.size (), mode);
  if (!shader)
    {
      paint.setColor (0);
      return;
    }

  shader->setLocalMatrix (qt2skia::matrix (m_transform));
  paint.setShader (shader)->unref ();
}

renderer_paint_server *renderer_linear_gradient::clone () const
{
  return new renderer_linear_gradient (*this);
}

void renderer_radial_gradient::fill_paint (SkPaint &paint) const
{
  SkPoint center = SkPoint::Make (SkFloatToScalar (m_cx), SkFloatToScalar (m_cy));

  unique_ptr<SkColor []> colors (new SkColor [m_stops.size ()]);
  unique_ptr<SkScalar[]> pos (new SkScalar [m_stops.size ()]);
  for (size_t i = 0; i < m_stops.size (); i++)
    {
      colors[i] = qt2skia::color (m_stops[i].second);
      pos[i] = SkFloatToScalar (m_stops[i].first);
    }

  SkShader::TileMode mode;
  switch (m_spread)
    {
    case spread_method::PAD: mode = SkShader::kClamp_TileMode; break;
    case spread_method::REFLECT: mode = SkShader::kMirror_TileMode; break;
    case spread_method::REPEAT: mode = SkShader::kRepeat_TileMode; break;
    case spread_method::INVALID: return;
    }

  /// standard says to stop rendering in case of 0 stops
  if (m_stops.size () == 0)
    {
      paint.setColor (0);
      return;
    }
  /// and to render with solid color in case of 1 stop
  else if (m_stops.size () == 1)
    {
      paint.setColor (colors[0]);
      return;
    }

  SkShader *shader = SkGradientShader::CreateRadial (center, SkFloatToScalar (m_r), colors.get (), pos.get (), (int)m_stops.size (), mode);
  if (!shader)
    {
      paint.setColor (0);
      return;
    }

  shader->setLocalMatrix (qt2skia::matrix (m_transform));
  paint.setShader (shader)->unref ();
}

renderer_paint_server *renderer_radial_gradient::clone () const
{
  return new renderer_radial_gradient (*this);
}
