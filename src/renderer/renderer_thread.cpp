#include "renderer/renderer_thread.h"

#include "common/memory_deallocation.h"
#include "common/math_defs.h"
#include "common/common_utils.h"

#include "renderer/abstract_renderer_event.h"
#include "renderer/abstract_renderer_item.h"
#include "renderer/svg_renderer.h"
#include "renderer/renderer_items_container.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/events_queue.h"
#include "renderer/renderer_config.h"

#include <QTransform>




renderer_thread::renderer_thread (svg_renderer *renderer, events_queue *queue, QObject *parent)
  : QThread (parent)
{
  m_renderer = renderer;
  m_queue = queue;
  m_container = nullptr;
  m_exit_needed = false;
}

renderer_thread::~renderer_thread ()
{
  FREE (m_container);
  FREE (m_renderer);
}

void renderer_thread::run ()
{
  int queue_id = 0;
  while (!m_exit_needed)
    {
      m_queue->lock ();
      if (m_queue->empty_nolock ())
        {
          m_queue->timed_wait (m_wait_time);
        }
      else
        {
          abstract_renderer_event *ev = m_queue->pull_event ();
          while (ev)
            {
              m_queue->unlock ();
              queue_id = ev->queue_id ();
              ev->process (this);
              FREE (ev);
              small_wait ();

              m_queue->lock ();
              ev = m_queue->pull_event ();
            }
        }
      m_queue->unlock ();
      redraw ();
      if (m_queue->empty ())
        {
          rendered_items_cache *cache = m_renderer->cache ();
          if (!cache->same_zoom (m_last_transform))
            cache->zoom_level_changed (m_last_transform.m11 (), m_last_transform.m22 ());
          m_queue->set_calculated_id (queue_id);
        }
    }

}

void renderer_thread::transform_changed (const render_cache_id &first, const render_cache_id &last, const QTransform &transform)
{
  m_first_id_to_change = first;
  m_last_id_to_change = last;
  m_last_transform = transform;
}

void renderer_thread::container_changed (renderer_items_container *container)
{
  FREE (m_container);
  m_container = container;
  rendered_items_cache *cache = m_renderer->cache ();
  cache->clear ();
}

void renderer_thread::redraw ()
{
  if (!m_container || m_first_id_to_change.object_type () == (int)render_cache_type::INVALID)
    return;

  m_queue->reset_interrupt ();
  rendered_items_cache *cache = m_renderer->cache ();
  bool new_cache = !cache->same_zoom (m_last_transform);
  if (new_cache)
    {
      cache->clear_next_zoom_cache ();
    }

  /// first, render actual image
  renderer_config cfg;
  cfg.set_use_new_cache (new_cache);
  m_renderer->update_cache_items (m_container->root (), m_first_id_to_change, m_last_id_to_change, m_last_transform, cfg);

  /// then, render image for fast selection
  cfg.set_render_for_selection (true);
  render_cache_id first (m_first_id_to_change.x (), m_first_id_to_change.y (), (int)render_cache_type::ROOT_ITEM_SELECTION);
  render_cache_id last (m_last_id_to_change.x (), m_last_id_to_change.y (), (int)render_cache_type::ROOT_ITEM_SELECTION);
  m_renderer->update_cache_items (m_container->root (), first, last, m_last_transform, cfg);
}

void renderer_thread::small_wait ()
{
  sleep_ms (1);
}

void renderer_thread::invalidate_rect (const QRectF &rect)
{
  QRectF pixel_rect = m_last_transform.mapRect (rect);
  auto object_pair = render_cache_id::get_id_for_pixel_rect (m_last_transform, pixel_rect, (int)render_cache_type::ROOT_ITEM);
  rendered_items_cache *cache = m_renderer->cache ();

  cache->remove_from_cache (object_pair.first, object_pair.second);
  render_cache_id selection_id_first = render_cache_id (object_pair.first.x (), object_pair.first.y (), (int)render_cache_type::ROOT_ITEM_SELECTION);
  render_cache_id selection_id_second = render_cache_id (object_pair.second.x (), object_pair.second.y (), (int)render_cache_type::ROOT_ITEM_SELECTION);
  cache->remove_from_cache (selection_id_first, selection_id_second);
}
