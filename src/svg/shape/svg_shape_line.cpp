#include "svg/shape/svg_shape_line.h"

#include <QPainterPath>

svg_shape_line::svg_shape_line (const point_2d &start, const point_2d &stop) : abstract_svg_shape (start, stop)
{

}

void svg_shape_line::add_to_path (QPainterPath &shape)
{
  shape.lineTo (stop ().x (), stop ().y ());
}
