#include "renderer/render_cache_id.h"

#include "renderer/rendered_items_cache.h"

#include <QRectF>
#include <QTransform>

static inline QTransform strip_translate (const QTransform &transform)
{
  return QTransform (transform.m11 (), 0, 0, transform.m22 (), 0, 0);
}

render_cache_id render_cache_id::get_id_by_pos (double x_pos, double y_pos, const QTransform &transform)
{
  double block_size_x = rendered_items_cache::block_pixel_size (), block_size_y = rendered_items_cache::block_pixel_size ();
  block_size_x /= transform.m11 ();
  block_size_y /= transform.m22 ();
  int x = (int)floor (x_pos / block_size_x);
  int y = (int)floor (y_pos / block_size_y);
  return render_cache_id (x, y, ROOT_ITEM);
}

QRectF render_cache_id::local_rect (const QTransform &transform) const
{
  double block_size_x = rendered_items_cache::block_pixel_size (), block_size_y = rendered_items_cache::block_pixel_size ();
  block_size_x /= transform.m11 ();
  block_size_y /= transform.m22 ();

  QRectF pix_rect = QRectF (m_x * block_size_x, m_y * block_size_y, block_size_x, block_size_y);
  return pix_rect;
}

QRectF render_cache_id::pixel_rect (const QTransform &transform) const
{
  return transform.mapRect (local_rect (transform));
}

