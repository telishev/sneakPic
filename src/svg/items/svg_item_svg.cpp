#include "svg/items/svg_item_svg.h"

#include "common/memory_deallocation.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attributes_length_type.h"

#include "renderer/renderer_item_svg.h"


svg_item_svg::svg_item_svg (svg_document *document)
  : abstract_svg_item (document)
{
  m_render_item = nullptr;
}

svg_item_svg::~svg_item_svg ()
{
  FREE (m_render_item);
}

double svg_item_svg::width () const
{
  static const svg_attribute_width default_width (nullptr, 210, svg_length_units::MM); /// A4
  return get_computed_attribute<svg_attribute_width> (&default_width)->value ();
}

double svg_item_svg::height () const
{
  static const svg_attribute_height default_height (nullptr, 297, svg_length_units::MM); /// A4
  return get_computed_attribute<svg_attribute_height> (&default_height)->value ();
}

void svg_item_svg::update_renderer_item ()
{
  if (!m_render_item)
    m_render_item = new renderer_item_svg;

  m_render_item->set_height (height ());
  m_render_item->set_width (width ());
}

const abstract_renderer_item *svg_item_svg::get_renderer_item () const
{
  return m_render_item;
}

