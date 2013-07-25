#include "common/math_defs.h"

#include "svg/items/svg_item_rect.h"

#include "svg/attributes/svg_attributes_length_type.h"

#include <QPainterPath>

svg_item_rect::svg_item_rect (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_rect::~svg_item_rect ()
{
}

bool svg_item_rect::check_item ()
{
  /// TODO: add check
  return true;
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

  double x_val = x->value ();
  double y_val = y->value ();
  double w_val = width->value ();
  double h_val = height->value ();

  QPainterPath path;
  if (are_equal (rx_val, 0.0) && are_equal (ry_val, 0.0))
    {
      path.addRect (x_val, y_val, w_val, h_val);
    }
  else
    {
      // Warning AddRoundRect is not used because of non svg standard point iteration order
      path.moveTo (x_val + rx_val, y_val);
      path.arcTo (x_val + w_val - 2 * rx_val, y_val, 2 * rx_val, 2 * ry_val, 90, -90);
      path.arcTo (x_val + w_val - 2 * rx_val, y_val + h_val - 2 * ry_val, 2 * rx_val, 2 * ry_val, 0, -90);
      path.arcTo (x_val, y_val + h_val - 2 * ry_val, 2 * rx_val, 2 * ry_val, -90, -90);
      path.arcTo (x_val, y_val, 2 * rx_val, 2 * ry_val, -180, -90);
      path.closeSubpath ();
    }
  return path;
}




