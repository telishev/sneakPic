#include "svg/items/svg_item_path.h"

#include <QPainterPath>

#include "svg/attributes/svg_attribute_path_data.h"


svg_item_path::svg_item_path (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_path::~svg_item_path ()
{
}

bool svg_item_path::check_item ()
{
  const svg_attribute_path_data *path_data = get_attribute<svg_attribute_path_data> ();
  if (!path_data)
    return true;

  return true;
}

QPainterPath svg_item_path::get_path () const 
{
  const svg_attribute_path_data *path_data = get_attribute<svg_attribute_path_data> ();

  if (!path_data)
    return QPainterPath ();

  return path_data->create_painter_path ();
}
