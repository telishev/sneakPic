#include "svg/items/svg_item_ellipse.h"

#include "renderer/renderer_item_ellipse.h"

#include "svg/attributes/svg_attributes_length_type.h"

svg_item_ellipse::svg_item_ellipse (svg_document *document)
  : svg_base_shape_item (document)
{
  m_render_item = nullptr;
}

svg_item_ellipse::~svg_item_ellipse ()
{
  FREE (m_render_item);
}

bool svg_item_ellipse::check_item ()
{
  return true;
}

void svg_item_ellipse::update_renderer_item ()
{
  const svg_attribute_cx *cx = get_computed_attribute<svg_attribute_cx> ();
  const svg_attribute_cy *cy = get_computed_attribute<svg_attribute_cy> ();
  const svg_attribute_rx *rx = get_computed_attribute<svg_attribute_rx> ();
  const svg_attribute_ry *ry = get_computed_attribute<svg_attribute_ry> ();

  if (!m_render_item)
    m_render_item = new renderer_item_ellipse;


  m_render_item->set_ellipse (cx->value (), cy->value (), rx->value (), ry->value ());
  update_base_item (m_render_item);
}

const abstract_renderer_item *svg_item_ellipse::get_renderer_item () const
{
  return m_render_item;
}



