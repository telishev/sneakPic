#include "renderer/render_cache_id.h"

#include "renderer/rendered_items_cache.h"

#include <QRectF>
#include <QTransform>

render_cache_id render_cache_id::get_id_by_pixel_pos (double x_pos, double y_pos, const QTransform &transform, int object_type)
{
  double block_size = rendered_items_cache::block_pixel_size ();
  int x = (int)floor ((x_pos - transform.dx ()) / block_size);
  int y = (int)floor ((y_pos - transform.dy ()) / block_size);
  return render_cache_id (x, y, object_type);
}

QRectF render_cache_id::pixel_rect (const QTransform &transform) const
{
  double block_size = rendered_items_cache::block_pixel_size ();
  double start_x = m_x * block_size + transform.dx ();
  double start_y = m_y * block_size + transform.dy ();
  return QRectF (start_x, start_y, block_size, block_size);
}

pair<render_cache_id, render_cache_id> render_cache_id::get_id_for_pixel_rect (const QTransform &transform, const QRectF &rect, int object_type)
{
  render_cache_id first = get_id_by_pixel_pos (rect.x (), rect.y (), transform, object_type);
  render_cache_id last = get_id_by_pixel_pos (rect.x () + rect.width (), rect.y () + rect.height (), transform, object_type);
  return std::make_pair (first, last);
}

