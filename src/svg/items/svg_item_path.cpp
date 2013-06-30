#include "svg_item_path.h"

#include <QPainterPath>
#include <QPainter>

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/svg_attribute_style.h"
#include "svg/attributes/svg_attribute_fill.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attribute_stroke.h"

#include "svg/shape/abstract_svg_shape.h"
#include "svg/shape/svg_subpath.h"




svg_item_path::svg_item_path (svg_document *document)
  : abstract_svg_item (document)
{
}

svg_item_path::~svg_item_path ()
{

}

void svg_item_path::draw (QPainter &painter)
{
  const svg_attribute_path_data *path_data = get_attribute<svg_attribute_path_data> ();
  if (!path_data)
    return;

  const std::vector<svg_subpath *> &subpath = path_data->subpath ();
  QPainterPath painter_path;
  for (svg_subpath *path : subpath)
    {
      const std::vector<abstract_svg_shape *> &shapes = path->shapes ();
      if (shapes.size () == 0)
        continue;

      painter_path.moveTo (shapes[0]->start ().x (), shapes[0]->start ().y ());
      for (abstract_svg_shape *shape : shapes)
        {
          shape->add_to_path (painter_path);
        }
    }

  draw_path (painter, painter_path);
}

void svg_item_path::draw_path (QPainter &painter, QPainterPath &painter_path)
{
  const svg_attribute_fill *fill = get_computed_attribute<svg_attribute_fill> ();
  const svg_attribute_stroke *stroke = get_computed_attribute<svg_attribute_stroke> ();
  const svg_attribute_stroke_width *stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();

  QPen pen;
  pen.setWidthF (stroke_width->get_stroke_width ());
  pen.setColor (stroke->get_color ());
  QBrush brush (fill->get_color ());
  if (fill->is_applied ())
    painter.fillPath (painter_path, brush);

  if (stroke->is_applied ())
    painter.strokePath (painter_path, pen);
}

bool svg_item_path::check_item ()
{
  const svg_attribute_path_data *path_data = get_attribute<svg_attribute_path_data> ();
  if (!path_data)
    return false;

  return true;
}
