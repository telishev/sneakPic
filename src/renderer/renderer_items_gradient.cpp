#include "renderer/renderer_items_gradient.h"

#include "svg/attributes/svg_attributes_enum.h"


void renderer_base_gradient_item::set_opacity (double opacity)
{
  QGradientStops stops = m_gradient.stops ();
  for (int i = 0; i < stops.size (); i++)
    stops[i].second.setAlphaF (opacity * stops[i].second.alphaF ());

  m_gradient.setStops (stops);
}

QBrush renderer_base_gradient_item::get_brush () const 
{
  return QBrush (m_gradient);
}

void renderer_base_gradient_item::add_stop (double position, const QColor &color)
{
  m_gradient.setColorAt (position, color);
}

void renderer_base_gradient_item::set_spread (spread_method spread)
{
  switch (spread)
    {
    case spread_method::PAD: m_gradient.setSpread (QGradient::PadSpread); break;
    case spread_method::REFLECT: m_gradient.setSpread (QGradient::ReflectSpread); break;
    case spread_method::REPEAT: m_gradient.setSpread (QGradient::RepeatSpread); break;
    case spread_method::INVALID: return;
    }
}

void renderer_base_gradient_item::set_gradient_units (gradient_units units)
{
  switch (units)
    {
    case gradient_units::OBJECT_BOUNDING_BOX: m_gradient.setCoordinateMode (QGradient::ObjectBoundingMode); break;
    case gradient_units::USER_SPACE : m_gradient.setCoordinateMode (QGradient::LogicalMode); break;
    case gradient_units::INVALID: return;
    }
}
