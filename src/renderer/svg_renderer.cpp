#include "svg_renderer.h"

#include <QPainter>
#include <QRectF>
#include <QTransform>

#include "common/common_utils.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/renderer_state.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/render_cache_id.h"
#include "renderer/renderer_config.h"

#include "svg/items/abstract_svg_item.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)


svg_renderer::svg_renderer (rendered_items_cache *cache, events_queue *queue)
{
  m_cache = cache;
  m_queue = queue;
}

svg_renderer::~svg_renderer ()
{

}

void svg_renderer::draw_item (const abstract_renderer_item *item, SkCanvas &canvas, const QRectF &rect_to_draw, const QTransform &transform)
{
  renderer_state state;
  state.set_rect (rect_to_draw.toRect ());
  state.set_transform (transform);
  renderer_config cfg (m_queue);
  item->draw (canvas, state, &cfg);
}

void svg_renderer::update_cache_item (const abstract_renderer_item *item, const render_cache_id &cache_id, const QTransform &transform,
                                      int total_x, int total_y, bool next_cache)
{
  int block_size = rendered_items_cache::block_pixel_size ();
  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, block_size * total_x, block_size * total_y);
  bitmap.allocPixels ();
  SkDevice device (bitmap);
  SkCanvas canvas (&device);
  canvas.drawColor (SK_ColorTRANSPARENT, SkXfermode::kSrc_Mode);

  QRectF local_rect = cache_id.pixel_rect (transform);
  QTransform last_inverted = transform.inverted ();
  QPointF last_pos_local = last_inverted.map (QPointF (0, 0));
  QPointF cur_pos_local = last_inverted.map (QPointF (local_rect.x (), local_rect.y ()));
  QPointF diff = -cur_pos_local + last_pos_local;

  QTransform pixmap_transform = QTransform (transform).translate (diff.x (), diff.y ());
  draw_item (item, canvas, QRectF (0, 0, block_size * total_x, block_size * total_y), pixmap_transform);

  if (total_x == total_y && total_x == 1)
    m_cache->add_bitmap (cache_id, bitmap, next_cache);
  else
    {
      for (int i = 0; i < total_x; i++)
        for (int j = 0; j < total_y; j++)
          {
            render_cache_id cur_id (cache_id.x () + i, cache_id.y () + j);
            SkBitmap bitmap_part;
            DEBUG_ASSERT (bitmap.extractSubset (&bitmap_part, SkIRect::MakeXYWH (i * block_size, j * block_size, block_size, block_size)));
            m_cache->add_bitmap (cur_id, bitmap_part, next_cache);
          }
    }
  
}

void svg_renderer::update_cache_items (const abstract_renderer_item *item, const render_cache_id &first,
                                       const render_cache_id &last, QTransform transform, bool next_cache)
{
  /// if nothing is cached, render everything in one pass
  if (!is_something_cached (first, last, next_cache))
    {
      update_cache_item (item, first, transform, last.x () - first.x () + 1,  last.y () - first.y () + 1, next_cache);
      return;
    }

  for (int x = first.x (); x <= last.x (); x++)
    for (int y = first.y (); y <= last.y (); y++)
      {
        render_cache_id id (x, y);
        if (m_cache->is_cached (id, next_cache))
          continue;

        update_cache_item (item, id, transform, 1, 1, next_cache);
      }
}

bool svg_renderer::is_something_cached (const render_cache_id &first, const render_cache_id &last, bool next_cache)
{
  for (int x = first.x (); x <= last.x (); x++)
    for (int y = first.y (); y <= last.y (); y++)
      {
        render_cache_id id (x, y);
        if (m_cache->is_cached (id, next_cache))
          return true;
      }

    return false;
}




