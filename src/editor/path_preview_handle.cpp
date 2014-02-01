#include "path_preview_handle.h"

#include <QPoint>
#include <QPointF>
#include <QTransform>
#include <QColor>

#include "path/svg_path_geom.h"

#include "svg/items/svg_item_path.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "renderer/path_preview_renderer.h"


path_preview_handle::path_preview_handle (svg_item_path *item)
{
  m_item = item;
}

path_preview_handle::~path_preview_handle ()
{

}

int path_preview_handle::distance_to_mouse (QPoint /*screen_pos*/, QTransform /*transform*/) const 
{
  return 10000;
}

void path_preview_handle::set_mouse_hovered (bool /*hovered*/)
{

}

bool path_preview_handle::start_drag (QPointF /*local_pos*/)
{
  return false;
}

bool path_preview_handle::drag (QPointF /*local_pos*/)
{
  return false;
}

bool path_preview_handle::end_drag (QPointF /*local_pos*/)
{
  return false;
}

void path_preview_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  QTransform transform = m_item->full_transform ();
  auto path_data = m_item->get_computed_attribute<svg_attribute_path_data> ();
  svg_path_geom path = *path_data->path ();
  path.apply_transform (transform);

  path_preview_renderer renderer (&path, QColor ("slateblue"));
  return renderer.draw (canvas, state, config);
}
