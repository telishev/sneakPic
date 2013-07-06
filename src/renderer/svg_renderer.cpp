#include "svg_renderer.h"

#include <QPainter>
#include <QRectF>
#include <QTransform>

#include "renderer/abstract_renderer_item.h"
#include "renderer/renderer_state.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/render_cache_id.h"

#include "svg/items/abstract_svg_item.h"
#include "common/common_utils.h"



svg_renderer::svg_renderer (rendered_items_cache *cache)
{
  m_cache = cache;
}

svg_renderer::~svg_renderer ()
{

}

void svg_renderer::draw_item (const abstract_svg_item *item, QPainter &painter, const QRectF &rect_to_draw, const QTransform &transform)
{
  const abstract_renderer_item *renderer_item = item->get_renderer_item ();
  if (!renderer_item)
    return;

  renderer_state state;
  state.set_rect (rect_to_draw.toRect ());
  state.set_transform (transform);
  renderer_item->draw (painter, state);
}

void svg_renderer::update_cache_item (const abstract_svg_item *item, const render_cache_id &cache_id, const QTransform &transform, int total_x, int total_y)
{
  int block_size = rendered_items_cache::block_pixel_size ();
  QPixmap pixmap (block_size * total_x, block_size * total_y);
  pixmap.fill (Qt::transparent);

  QPainter painter (&pixmap);
  QRectF local_rect = cache_id.pixel_rect (transform);
  QTransform last_inverted = transform.inverted ();
  QPointF last_pos_local = last_inverted.map (QPointF (0, 0));
  QPointF cur_pos_local = last_inverted.map (QPointF (local_rect.x (), local_rect.y ()));
  QPointF diff = -cur_pos_local + last_pos_local;

  QTransform pixmap_transform = QTransform (transform).translate (diff.x (), diff.y ());
  draw_item (item, painter, pixmap.rect (), pixmap_transform);

  painter.end ();
  if (total_x == total_y && total_x == 1)
    m_cache->add_pixmap (cache_id, pixmap);
  else
    {
      for (int i = 0; i < total_x; i++)
        for (int j = 0; j < total_y; j++)
          {
            render_cache_id cur_id (cache_id.x () + i, cache_id.y () + j);
            m_cache->add_pixmap (cur_id, pixmap.copy (i * block_size, j * block_size, block_size, block_size));
          }
    }
  
}

void svg_renderer::update_cache_items (const abstract_svg_item *item, const render_cache_id &first,
                                       const render_cache_id &last, QTransform transform)
{
  /// if nothing is cached, render everything in one pass
  if (!is_something_cached (first, last))
    {
      update_cache_item (item, first, transform, last.x () - first.x () + 1,  last.y () - first.y () + 1);
      return;
    }

  for (int x = first.x (); x <= last.x (); x++)
    for (int y = first.y (); y <= last.y (); y++)
      {
        render_cache_id id (x, y);
        if (m_cache->is_cached (id))
          continue;

        update_cache_item (item, id, transform, 1, 1);
      }
}

bool svg_renderer::is_something_cached (const render_cache_id &first, const render_cache_id &last)
{
  for (int x = first.x (); x <= last.x (); x++)
    for (int y = first.y (); y <= last.y (); y++)
      {
        render_cache_id id (x, y);
        if (m_cache->is_cached (id))
          return true;
      }

    return false;
}




