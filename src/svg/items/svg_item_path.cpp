#include "svg/items/svg_item_path.h"

#include <QPainterPath>

#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "svg/attributes/svg_attribute_linetypes.h"

#include "path/svg_path_geom.h"


svg_item_path::svg_item_path (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_path::~svg_item_path ()
{
}

bool svg_item_path::process_item_after_read ()
{
  auto *path_data = get_computed_attribute<svg_attribute_path_data> ();
  auto nodetypes = get_attribute_for_change<svg_attribute_nodetypes> ();
  auto linetypes = get_attribute_for_change<svg_attribute_linetypes> ();

  nodetypes->create_from_path (path_data->path (), true);
  linetypes->create_from_path (path_data->path (), true);

  return true;
}

QPainterPath svg_item_path::get_path () const 
{
  const svg_attribute_path_data *path_data = get_computed_attribute<svg_attribute_path_data> ();

  if (!path_data)
    return QPainterPath ();

  return path_data->create_painter_path ();
}

const svg_path_geom * svg_item_path::get_svg_path () const
{
  return get_computed_attribute<svg_attribute_path_data> ()->path ();
}
