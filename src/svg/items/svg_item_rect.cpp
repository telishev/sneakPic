#include "svg/items/svg_item_rect.h"

#include "renderer/renderer_item_rect.h"

#include "svg/attributes/svg_attributes_length_type.h"



svg_item_rect::svg_item_rect (svg_document *document)
  : svg_base_shape_item (document)
{
  m_render_item = nullptr;
}

svg_item_rect::~svg_item_rect ()
{
  FREE (m_render_item);
}

bool svg_item_rect::check_item ()
{
  /// TODO: add check
  return true;
}

void svg_item_rect::update_renderer_item ()
{
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  const svg_attribute_rx *rx = get_computed_attribute<svg_attribute_rx> ();
  const svg_attribute_ry *ry = get_computed_attribute<svg_attribute_ry> ();
  const svg_attribute_width *width = get_computed_attribute<svg_attribute_width> ();
  const svg_attribute_height *height = get_computed_attribute<svg_attribute_height> ();

  double rx_val = rx->value (), ry_val = ry->value ();

  if (rx->is_empty () && !ry->is_empty ())
    rx_val = ry_val;
  else if (!rx->is_empty () && ry->is_empty ())
    ry_val = rx_val;

  if (rx_val > width->value () / 2)
    rx_val = width->value () / 2;

  if (ry_val > height->value () / 2)
    ry_val = height->value () / 2;

  if (!m_render_item)
    m_render_item = new renderer_item_rect;


  m_render_item->set_rect (x->value (), y->value (), width->value (), height->value (), rx_val, ry_val);
  update_base_item (m_render_item);
}

const abstract_renderer_item *svg_item_rect::get_renderer_item () const
{
  return m_render_item;
}

QPainterPath svg_item_rect::get_path () const 
{
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute<svg_attribute_y> ();
  const svg_attribute_rx *rx = get_computed_attribute<svg_attribute_rx> ();
  const svg_attribute_ry *ry = get_computed_attribute<svg_attribute_ry> ();
  const svg_attribute_width *width = get_computed_attribute<svg_attribute_width> ();
  const svg_attribute_height *height = get_computed_attribute<svg_attribute_height> ();

  double rx_val = rx->value (), ry_val = ry->value ();

  if (rx->is_empty () && !ry->is_empty ())
    rx_val = ry_val;
  else if (!rx->is_empty () && ry->is_empty ())
    ry_val = rx_val;

  if (rx_val > width->value () / 2)
    rx_val = width->value () / 2;

  if (ry_val > height->value () / 2)
    ry_val = height->value () / 2;

  QPainterPath path;
  path.addRoundedRect (x->value (), y->value (), width->value (), height->value (), rx_val, ry_val);
  return path;
}




