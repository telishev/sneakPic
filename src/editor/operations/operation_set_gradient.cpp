#include "operation_set_gradient.h"

#include "svg\attributes\svg_attributes_fill_stroke.h"
#include "svg\attributes\svg_attribute_xlink_href.h"

#include "svg\items\svg_items_container.h"
#include "svg\items\svg_base_items_gradient.h"
#include "svg\items\svg_item_linear_gradient.h"
#include "svg\items\svg_item_radial_gradient.h"

#include "svg\svg_document.h"

#include "renderer\renderer_paint_server.h"
#include "renderer\renderer_items_gradient.h"
#include "svg\attributes\svg_attributes_enum.h"
#include "svg\attributes\svg_attribute_gradient_transform.h"
#include "svg\items\svg_item_stop.h"
#include "svg\attributes\svg_attribute_stop_color.h"
#include "svg\attributes\svg_attributes_number.h"
#include "svg\attributes\svg_attribute_offset.h"
#include "svg\attributes\svg_attributes_length_type.h"
#include "svg\items\abstract_svg_item.h"



operation_set_gradient::operation_set_gradient (const renderer_base_gradient_item *gradient)
{
  m_gradient = gradient;
}

operation_set_gradient::~operation_set_gradient ()
{

}

void operation_set_gradient::apply (svg_document *document, svg_paint_server *paint_server)
{
  svg_base_items_gradient *cur_gradient = get_suitable_gradient (paint_server, document->item_container ());
  if (!cur_gradient)
    {
      if (m_gradient->type () == renderer_paint_server_type::LINEAR_GRADIENT)
        cur_gradient = document->create_new_svg_item<svg_item_linear_gradient> ();
      else
        cur_gradient = document->create_new_svg_item<svg_item_radial_gradient> ();

      /// TODO: add to defs
      document->root ()->push_back (cur_gradient);
    }

  paint_server->set_to_iri (QString::fromStdString (cur_gradient->name ()));
  fill_gradient (cur_gradient);
  add_stops_to_gradient (document, cur_gradient);
  if (m_gradient->type () == renderer_paint_server_type::LINEAR_GRADIENT)
    fill_linear_gradient (cur_gradient);
  else
    fill_radial_gradient (cur_gradient);
}

svg_base_items_gradient *operation_set_gradient::get_suitable_gradient (svg_paint_server *server, svg_items_container *container) const
{
  if (server->server_type () != paint_server_type::IRI)
    return nullptr;

  std::string fragment_name = server->iri ().get_fragment_name ();
  svg_base_items_gradient *base_gradient = dynamic_cast<svg_base_items_gradient *> (container->get_item (fragment_name));
  if (!base_gradient)
    return nullptr;
             
  if (m_gradient->type () == renderer_paint_server_type::LINEAR_GRADIENT && base_gradient->gradient_type () == gradient_type_t::LINEAR)
    return base_gradient;
  else if (m_gradient->type () == renderer_paint_server_type::RADIAL_GRADIENT && base_gradient->gradient_type () == gradient_type_t::RADIAL)
    return base_gradient;
  else
    return nullptr;

}

void operation_set_gradient::fill_gradient (svg_base_items_gradient *gradient)
{
  gradient->remove_attribute (gradient->get_computed_attribute<svg_attribute_xlink_href> ());
  gradient->get_attribute_for_change<svg_attribute_gradient_units> ()->set_value (m_gradient->units ());
  gradient->get_attribute_for_change<svg_attribute_spread_method> ()->set_value (m_gradient->spread ());
  gradient->get_attribute_for_change<svg_attribute_gradient_transform> ()->set_transform (m_gradient->transform ());
}

void operation_set_gradient::add_stops_to_gradient (svg_document *document, svg_base_items_gradient *gradient)
{
  gradient->remove_children ();
  for (auto stop_value : m_gradient->stops ())
    {
      svg_item_stop *stop = document->create_new_svg_item<svg_item_stop> ();
      gradient->push_back (stop);
      stop->get_attribute_for_change<svg_attribute_stop_color> ()->set_stop_color (stop_value.second);
      stop->get_attribute_for_change<svg_attribute_stop_opacity> ()->set_value (stop_value.second.alphaF ());
      stop->get_attribute_for_change<svg_attribute_offset> ()->set_offset (stop_value.first);
    }
}

void operation_set_gradient::fill_linear_gradient (svg_base_items_gradient *gradient)
{
  const renderer_linear_gradient *lin_grad = static_cast<const renderer_linear_gradient *> (m_gradient);
  gradient->get_attribute_for_change<svg_attribute_x1> ()->set_value (lin_grad->x1 ());
  gradient->get_attribute_for_change<svg_attribute_y1> ()->set_value (lin_grad->y1 ());
  gradient->get_attribute_for_change<svg_attribute_x2> ()->set_value (lin_grad->x2 ());
  gradient->get_attribute_for_change<svg_attribute_y2> ()->set_value (lin_grad->y2 ());
}

void operation_set_gradient::fill_radial_gradient (svg_base_items_gradient *gradient)
{
  const renderer_radial_gradient *rad_grad = static_cast<const renderer_radial_gradient *> (m_gradient);
  gradient->get_attribute_for_change<svg_attribute_cx> ()->set_value (rad_grad->cx ());
  gradient->get_attribute_for_change<svg_attribute_cy> ()->set_value (rad_grad->cy ());
  gradient->get_attribute_for_change<svg_attribute_fx> ()->set_value (rad_grad->fx ());
  gradient->get_attribute_for_change<svg_attribute_fy> ()->set_value (rad_grad->fy ());
  gradient->get_attribute_for_change<svg_attribute_r> ()->set_value (rad_grad->r ());
}
