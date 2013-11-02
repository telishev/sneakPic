#include "svg_renderer.h"

#include <QPainter>
#include <QRectF>
#include <QTransform>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include "common/common_utils.h"
#include "common/debug_utils.h"
#include "common/math_defs.h"


#include "renderer/renderable_item.h"
#include "renderer/renderer_state.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/render_cache_id.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"
#include "renderer/events_queue.h"



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

void svg_renderer::draw_item (const renderable_item *item, SkCanvas &canvas, const renderer_state &state, renderer_config &cfg)
{
  cfg.set_queue (m_queue);
  item->draw (canvas, state, &cfg);
}

void svg_renderer::update_cache_item (const renderable_item *item, const render_cache_id &cache_id, const QTransform &transform,
                                      renderer_config &cfg, int total_x, int total_y)
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
  renderer_state state (QRect (0, 0, block_size * total_x, block_size * total_y), pixmap_transform);
  draw_item (item, canvas, state, cfg);

  if (m_queue->is_interrupted ())
    return;

  if (total_x == total_y && total_x == 1)
    m_cache->add_bitmap (cache_id, bitmap, cfg.use_new_cache ());
  else
    {
      for (int i = 0; i < total_x; i++)
        for (int j = 0; j < total_y; j++)
          {
            render_cache_id cur_id (cache_id.x () + i, cache_id.y () + j, cache_id.object_type ());
            SkBitmap bitmap_part;
            DEBUG_ASSERT (bitmap.extractSubset (&bitmap_part, SkIRect::MakeXYWH (i * block_size, j * block_size, block_size, block_size)));
            m_cache->add_bitmap (cur_id, bitmap_part, cfg.use_new_cache ());
          }
    }

}

void svg_renderer::update_cache_items (const renderable_item *item, const render_cache_id &first,
                                       const render_cache_id &last, QTransform transform, renderer_config &cfg)
{
  DEBUG_ASSERT (first.object_type () == last.object_type ());

  /// if nothing is cached, render everything in one pass
  if (!is_something_cached (first, last, cfg))
    {
      update_cache_item (item, first, transform, cfg, last.x () - first.x () + 1,  last.y () - first.y () + 1);
      return;
    }

  QList<QFuture<void>> futures_list;
  for (int x = first.x (); x <= last.x (); x++)
    for (int y = first.y (); y <= last.y (); y++)
      {
        render_cache_id id (x, y, first.object_type ());
        if (m_cache->is_cached (id, cfg.use_new_cache ()))
          continue;

        futures_list.push_back (QtConcurrent::run (this, &svg_renderer::update_cache_item_async, item, id, transform, cfg));
        update_cache_item (item, id, transform, cfg, 1, 1);
      }

  for (int i = 0; i < futures_list.size (); i++)
    futures_list[i].waitForFinished ();
}

bool svg_renderer::is_something_cached (const render_cache_id &first, const render_cache_id &last, renderer_config &cfg)
{
  for (int x = first.x (); x <= last.x (); x++)
    for (int y = first.y (); y <= last.y (); y++)
      {
        render_cache_id id (x, y, first.object_type ());
        if (m_cache->is_cached (id, cfg.use_new_cache ()))
          return true;
      }

    return false;
}

void svg_renderer::update_cache_item_async (const renderable_item *item, const render_cache_id &cache_id, const QTransform &transform, renderer_config &cfg)
{
  return update_cache_item (item, cache_id, transform, cfg, 1, 1);
}

SkBitmap *svg_renderer::draw_to_bitmap (const QRect &rect_to_draw, const QTransform &transform, const renderable_item *item)
{
  SkBitmap *bitmap = new SkBitmap;
  bitmap->setConfig (SkBitmap::kARGB_8888_Config, rect_to_draw.width (), rect_to_draw.height ());
  bitmap->allocPixels ();
  bitmap->eraseColor (SK_ColorTRANSPARENT);
  draw_to_bitmap (rect_to_draw, transform, item, bitmap);
  return bitmap;
}

void svg_renderer::draw_to_bitmap (const QRect &rect_to_draw, const QTransform &transform, const renderable_item *item, SkBitmap *bitmap)
{
  SkDevice device (*bitmap);
  SkCanvas canvas (&device);

  svg_renderer renderer (nullptr, nullptr);
  renderer_state state (rect_to_draw, transform);
  renderer_config cfg;
  renderer.draw_item (item, canvas, state, cfg);
}

void svg_renderer::update_drawing (const QTransform &transform, const QRectF &rect_to_update, int cache_object_type)
{
  m_cache->lock ();
  DO_ON_EXIT (m_cache->unlock ());

  double cache_zoom_x = m_cache->zoom_x ();
  double cache_zoom_y = m_cache->zoom_y ();
  double cur_zoom_x = transform.m11 ();
  double cur_zoom_y = transform.m22 ();
  QRectF rect_to_draw = rect_to_update;
  QTransform real_transform = transform;

  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, rect_to_draw.width (), rect_to_draw.height ());
  bitmap.allocPixels ();
  SkDevice device (bitmap);
  SkCanvas canvas (&device);

  if (!are_equal (cache_zoom_x, cur_zoom_x) || !are_equal (cache_zoom_y, cur_zoom_y))
    {
      QTransform scale_transform = QTransform::fromScale (cache_zoom_x / cur_zoom_x, cache_zoom_y / cur_zoom_y);
      real_transform = real_transform * scale_transform;
      canvas.setMatrix (qt2skia::matrix (scale_transform.inverted ()));
      rect_to_draw = scale_transform.mapRect (rect_to_draw);
    }

  canvas.drawColor (SK_ColorTRANSPARENT, SkXfermode::kSrc_Mode);

  std::pair<render_cache_id, render_cache_id> it_pair = render_cache_id::get_id_for_pixel_rect (real_transform, rect_to_draw, cache_object_type);

  for (int x = it_pair.first.x (); x <= it_pair.second.x (); x++)
    for (int y = it_pair.first.y (); y <= it_pair.second.y (); y++)
      {
        render_cache_id cur_id (x, y, cache_object_type);
        SkBitmap bitmap = m_cache->bitmap (cur_id);
        if (bitmap.empty ())
          continue;

        QRectF pixel_rect = cur_id.pixel_rect (real_transform);
        canvas.drawBitmap (bitmap, SkFloatToScalar (pixel_rect.x ()), SkFloatToScalar (pixel_rect.y ()));
      }

  m_cache->set_current_screen (bitmap, cache_object_type);
}
