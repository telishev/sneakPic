#include "renderer/renderer_item_svg.h"

#include <QPainter>

#include "renderer/renderer_state.h"


renderer_item_svg::renderer_item_svg (const abstract_svg_item *m_svg_item)
  : renderer_item_group (m_svg_item)
{
  m_height = m_width = 0.0;

}

renderer_item_svg::~renderer_item_svg ()
{

}

void renderer_item_svg::draw (QPainter &painter, const renderer_state &state) const
{
  /// TODO: render document boundaries
  painter.setTransform (state.transform ());
  painter.drawRect (QRectF (0.0, 0.0, m_width, m_height));

  renderer_item_group::draw (painter, state);
}

void renderer_item_svg::set_height (double height)
{
  m_height = height;
}

void renderer_item_svg::set_width (double width)
{
  m_width = width;
}

