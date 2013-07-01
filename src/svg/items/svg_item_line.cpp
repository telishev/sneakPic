#include "svg/items/svg_item_line.h"

#include "renderer/renderer_item_line.h"

#include "svg/attributes/svg_attributes_length_type.h"

svg_item_line::svg_item_line (svg_document *document)
  : svg_base_shape_item (document)
{
  m_render_item = nullptr;
}

svg_item_line::~svg_item_line ()
{
  FREE (m_render_item);
}

bool svg_item_line::check_item ()
{
  return true;
}

void svg_item_line::update_renderer_item ()
{
  const svg_attribute_x1 *x1 = get_computed_attribute<svg_attribute_x1> ();
  const svg_attribute_x2 *x2 = get_computed_attribute<svg_attribute_x2> ();
  const svg_attribute_y1 *y1 = get_computed_attribute<svg_attribute_y1> ();
  const svg_attribute_y2 *y2 = get_computed_attribute<svg_attribute_y2> ();

  if (!m_render_item)
    m_render_item = new renderer_item_line;


  m_render_item->set_line (x1->value (), y1->value (), x2->value (), y2->value ());
  update_base_item (m_render_item);
}

const abstract_renderer_item *svg_item_line::get_renderer_item () const
{
  return m_render_item;
}




