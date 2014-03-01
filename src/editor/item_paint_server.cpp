#include "item_paint_server.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attributes_number.h"
#include "items_selection.h"

item_paint_server::item_paint_server ()
{
  m_type = item_paint_server_type::COLOR;
  m_color = Qt::black;
}

item_paint_server::~item_paint_server ()
{

}

void item_paint_server::create_from_item (abstract_svg_item *item, bool is_fill)
{
  const svg_paint_server *paint_server = nullptr;
  double opacity = 0.0;
  m_type = item_paint_server_type::NONE;
  m_color = QColor ();
  m_gradient = QGradient ();
  if (is_fill)
    {
      paint_server = item->get_computed_attribute <svg_attribute_fill> ();
      opacity =  item->get_computed_attribute <svg_attribute_fill_opacity> ()->value ();
    }
  else
    {
      paint_server = item->get_computed_attribute <svg_attribute_stroke> ();
      opacity =  item->get_computed_attribute <svg_attribute_stroke_opacity> ()->value ();
    }

  if (paint_server == nullptr)
    return;

  paint_server_type fill_type = paint_server->server_type ();
  switch (fill_type)
    {
    case paint_server_type::COLOR:
      m_type = item_paint_server_type::COLOR;
      m_color = paint_server->color ();
      m_color.setAlphaF (opacity);
      break;
    case paint_server_type::NONE:
    case paint_server_type::IRI:
    case paint_server_type::CURRENT_COLOR:
      break;
    }
}

void item_paint_server::apply_to_item (abstract_svg_item *item, bool is_fill) const
{
  if (is_fill)
    {
      item->get_attribute_for_change <svg_attribute_fill_opacity> ()->set_value (m_color.alphaF ());
      auto fill = item->get_attribute_for_change <svg_attribute_fill> ();
      if (m_type == item_paint_server_type::COLOR)
        fill->set_to_color (m_color);
      else if (m_type == item_paint_server_type::NONE)
        fill->set_to_none ();
    }
  else
    {
      item->get_attribute_for_change <svg_attribute_stroke_opacity> ()->set_value (m_color.alphaF ());
      auto stroke = item->get_attribute_for_change <svg_attribute_stroke> ();
      if (m_type == item_paint_server_type::COLOR)
        stroke->set_to_color (m_color);
      else if (m_type == item_paint_server_type::NONE)
        stroke->set_to_none ();
    }
}

void item_paint_server::create_from_selection (items_selection *selection, bool is_fill)
{
  abstract_svg_item *item = selection->single_item ();
  if (!item)
    return;

  create_from_item (item, is_fill);
}

void item_paint_server::apply_to_selection (items_selection *selection, bool is_fill) const
{
  for (abstract_svg_item *item : *selection)
    apply_to_item (item, is_fill);
}

void item_paint_server::set_color (QColor color)
{
  m_color = color;
  m_type = item_paint_server_type::COLOR;
}

QColor item_paint_server::color () const
{
  return m_color;
}
