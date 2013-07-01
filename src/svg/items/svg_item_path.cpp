#include "svg/items/svg_item_path.h"

#include <QPainterPath>

#include "renderer/renderer_item_path.h"

#include "svg/attributes/svg_attribute_path_data.h"

#include "svg/shape/abstract_svg_shape.h"
#include "svg/shape/svg_subpath.h"


svg_item_path::svg_item_path (svg_document *document)
  : svg_base_shape_item (document)
{
  m_render_item = nullptr;
}

svg_item_path::~svg_item_path ()
{
  FREE (m_render_item);
}

bool svg_item_path::check_item ()
{
  const svg_attribute_path_data *path_data = get_attribute<svg_attribute_path_data> ();
  if (!path_data)
    return true;

  return true;
}

void svg_item_path::update_renderer_item ()
{
  const svg_attribute_path_data *path_data = get_attribute<svg_attribute_path_data> ();

  if (!path_data)
    return;

  if (!m_render_item)
    m_render_item = new renderer_item_path;

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

  m_render_item->set_painter_path (painter_path);

  update_base_item (m_render_item);
}

const abstract_renderer_item *svg_item_path::get_renderer_item () const
{
  return m_render_item;
}

