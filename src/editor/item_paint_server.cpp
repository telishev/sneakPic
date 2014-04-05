#include "item_paint_server.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attributes_number.h"
#include "items_selection.h"
#include "renderer/renderer_paint_server.h"
#include "renderer/renderer_items_gradient.h"
#include "svg/items/svg_base_items_gradient.h"
#include "svg/items/svg_items_container.h"
#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/svg_document.h"
#include "svg/items/svg_item_linear_gradient.h"
#include "operations/operation_apply_paint_server.h"
#include "svg/items/svg_graphics_item.h"

item_paint_server::item_paint_server ()
{
  m_current_type = renderer_paint_server_type::COLOR;
  put_in (m_color, Qt::black);
  put_in (m_linear_gradient, 0.0, 0.5, 1.0, 0.5);
  m_linear_gradient->add_stop (0, Qt::transparent);
  m_linear_gradient->add_stop (1, Qt::black);

  put_in (m_radial_gradient, 0.5, 0.5, 0.5, 0.5, 0.5);
  m_radial_gradient->add_stop (0, Qt::black);
  m_radial_gradient->add_stop (1, Qt::transparent);

  put_in (m_none);
}

item_paint_server::item_paint_server (const item_paint_server &rhs)
{
  *this = rhs;
}

item_paint_server::item_paint_server (item_paint_server &&rhs)
{
  *this = std::move (rhs);
}

item_paint_server::~item_paint_server ()
{

}

void item_paint_server::create_from_item (abstract_svg_item *item, bool is_fill)
{
  const svg_paint_server *paint_server = nullptr;
  double opacity = 0.0;
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
    {
      m_current_type = renderer_paint_server_type::NONE;
      return;
    }

  unique_ptr<renderer_paint_server> renderer_server (paint_server->create_paint_server (item->document ()->item_container ()));
  renderer_server->set_opacity (opacity);
  if (renderer_server->type () == renderer_paint_server_type::LINEAR_GRADIENT ||
      renderer_server->type () == renderer_paint_server_type::RADIAL_GRADIENT)
    {
      if (item->to_graphics_item ())
        {
          auto graphics_item = item->to_graphics_item ();
          auto grad = static_cast<renderer_base_gradient_item *> (renderer_server.get ());
          grad->convert_to_bbox_units (graphics_item->full_transform ().inverted ().mapRect (graphics_item->bbox ()));
        }
    }

  set_current_server (renderer_server.get ());
}

void item_paint_server::apply_to_item (abstract_svg_item *item, bool is_fill) const
{
  operation_apply_paint_server (current_server (), is_fill).apply (item);
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
  m_color->set_color (color);
  m_current_type = renderer_paint_server_type::COLOR;
}

QColor item_paint_server::color () const
{
  return m_color->color ();
}

item_paint_server& item_paint_server::operator= (const item_paint_server &rhs)
{
  m_color.reset (rhs.m_color->clone ());
  m_linear_gradient.reset (rhs.m_linear_gradient->clone ());
  m_radial_gradient.reset (rhs.m_radial_gradient->clone ());
  m_none.reset (rhs.m_none->clone ());
  m_current_type = rhs.m_current_type;
  return *this;
}

item_paint_server& item_paint_server::operator= (item_paint_server &&rhs)
{
  m_color = std::move (rhs.m_color);
  m_linear_gradient = std::move (rhs.m_linear_gradient);
  m_radial_gradient = std::move (rhs.m_radial_gradient);
  m_none = std::move (rhs.m_none);
  m_current_type = rhs.m_current_type;
  return *this;
}

const renderer_paint_server * item_paint_server::current_server () const
{
  return server_for_type (m_current_type);
}

void item_paint_server::set_linear_gradient (const renderer_linear_gradient &linear_grad)
{
  m_linear_gradient.reset (linear_grad.clone ());
  m_current_type = renderer_paint_server_type::LINEAR_GRADIENT;
}

const renderer_linear_gradient * item_paint_server::linear_gradient () const
{
  return m_linear_gradient.get ();
}

void item_paint_server::set_radial_gradient (const renderer_radial_gradient &radial_grad)
{
  m_radial_gradient.reset (radial_grad.clone ());
  m_current_type = renderer_paint_server_type::RADIAL_GRADIENT;
}

const renderer_radial_gradient * item_paint_server::radial_gradient () const
{
  return m_radial_gradient.get ();
}

const renderer_paint_server * item_paint_server::server_for_type (renderer_paint_server_type type) const
{
  switch (type)
    {
    case renderer_paint_server_type::COLOR: return m_color.get ();
    case renderer_paint_server_type::NONE: return m_none.get ();
    case renderer_paint_server_type::LINEAR_GRADIENT: return m_linear_gradient.get ();
    case renderer_paint_server_type::RADIAL_GRADIENT: return m_radial_gradient.get ();
    }

  return nullptr;
}

void item_paint_server::set_current_server (const renderer_paint_server *server)
{
  m_current_type = server->type ();
  unique_ptr<renderer_paint_server> renderer_server (server->clone ());
  
  switch (renderer_server->type ())
    {
    case renderer_paint_server_type::NONE: break;
    case renderer_paint_server_type::COLOR:
      {
        m_color.reset (static_cast<renderer_painter_server_color *> (renderer_server.release ()));
        break;
      }
    case renderer_paint_server_type::LINEAR_GRADIENT:
      {
          m_linear_gradient.reset (static_cast<renderer_linear_gradient *> (renderer_server.release ()));
          break;
      }
    case renderer_paint_server_type::RADIAL_GRADIENT:
      {
        m_radial_gradient.reset (static_cast<renderer_radial_gradient *> (renderer_server.release ()));
        break;
      }
    }
}

renderer_base_gradient_item * item_paint_server::get_gradient_for_change ()
{
  switch (m_current_type)
    {
    case renderer_paint_server_type::COLOR: return nullptr;
    case renderer_paint_server_type::NONE: return nullptr;
    case renderer_paint_server_type::LINEAR_GRADIENT: return m_linear_gradient.get ();
    case renderer_paint_server_type::RADIAL_GRADIENT: return m_radial_gradient.get ();
    }

  return nullptr;
}
