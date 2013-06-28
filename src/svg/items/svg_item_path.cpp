#include "svg_item_path.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/svg_attribute_style.h"

#include "svg/shape/abstract_svg_shape.h"
#include "svg/shape/svg_subpath.h"

#include <QPainterPath>
#include <QPainter>

svg_item_path::svg_item_path (svg_document *document)
  : abstract_svg_item (document)
{
  m_path_data = nullptr;
  m_path_style = nullptr;
}

svg_item_path::~svg_item_path ()
{

}

void svg_item_path::process_attribute (abstract_attribute *attribute)
{
  switch (attribute->type ())
    {
    case svg_attribute_type::STYLE:
      m_path_style = static_cast<svg_attribute_style *> (attribute);
      break;
    case svg_attribute_type::D:
      m_path_data = static_cast<svg_attribute_path_data *> (attribute);
      break;
    default:
      break;
    }

  return abstract_svg_item::process_attribute (attribute);
}

void svg_item_path::draw (QPainter &painter)
{
  const std::vector<svg_subpath *> &subpath = m_path_data->subpath ();
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

  painter.drawPath (painter_path);
}
