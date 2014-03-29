#include "operation_apply_paint_server.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"

#include "renderer/renderer_items_gradient.h"
#include "renderer/renderer_paint_server.h"

#include "operation_set_gradient.h"

#include "editor/item_paint_server.h"





operation_apply_paint_server::operation_apply_paint_server (const renderer_paint_server *server, bool is_fill)
{
  m_server = server;
  m_is_fill = is_fill;
}

operation_apply_paint_server::~operation_apply_paint_server ()
{

}

void operation_apply_paint_server::apply (abstract_svg_item *item)
{
  if (m_is_fill)
    {
      auto opacity = item->get_attribute_for_change <svg_attribute_fill_opacity> ();
      auto fill = item->get_attribute_for_change <svg_attribute_fill> ();
      set_data (*fill, *opacity, item);
    }
  else
    {
      auto opacity = item->get_attribute_for_change <svg_attribute_stroke_opacity> ();
      auto stroke = item->get_attribute_for_change <svg_attribute_stroke> ();
      set_data (*stroke, *opacity, item);
    }
}

void operation_apply_paint_server::set_data (svg_paint_server &server, svg_base_attribute_number &opacity, abstract_svg_item *item) const
{
  switch (m_server->type ())
    {
    case renderer_paint_server_type::NONE: server.set_to_none (); break;
    case renderer_paint_server_type::COLOR:
      {
        const renderer_painter_server_color *color = static_cast<const renderer_painter_server_color *> (m_server);
        opacity.set_value (color->color ().alphaF ());
        server.set_to_color (color->color ());
        break;
      }
    case renderer_paint_server_type::LINEAR_GRADIENT:
    case renderer_paint_server_type::RADIAL_GRADIENT:
      {
        operation_set_gradient (static_cast<const renderer_base_gradient_item *> (m_server)).apply (item->document (), &server);
        break;
      }
    default:
      break;
    }
}
