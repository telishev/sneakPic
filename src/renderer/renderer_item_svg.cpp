#include "renderer/renderer_item_svg.h"


renderer_item_svg::renderer_item_svg ()
{
  m_height = m_width = 0.0;

}

renderer_item_svg::~renderer_item_svg ()
{

}

void renderer_item_svg::draw (QPainter &/*painter*/) const
{
  /// TODO: render document boundaries
}

void renderer_item_svg::set_height (double height)
{
  m_height = height;
}

void renderer_item_svg::set_width (double width)
{
  m_width = width;
}
