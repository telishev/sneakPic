#include "renderer/renderer_thread.h"

#include "common/memory_deallocation.h"
#include "common/math_defs.h"
#include "common/common_utils.h"

#include "renderer/abstract_renderer_event.h"
#include "renderer/svg_renderer.h"
#include "renderer/renderer_items_container.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/events_queue.h"

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
      if (m_queue->empty ())
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
      m_queue->set_calculated_id (queue_id);
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
}

void renderer_thread::redraw ()
{
  if (!m_container)
    return;

  rendered_items_cache *cache = m_renderer->cache ();
  double cache_zoom_x = cache->zoom_x ();
  double cache_zoom_y = cache->zoom_x ();
  double cur_zoom_x = m_last_transform.m11 ();
  double cur_zoom_y = m_last_transform.m22 ();
  bool new_cache = false;
  if (!are_equal (cache_zoom_x, cur_zoom_x) || !are_equal (cache_zoom_y, cur_zoom_y))
    {
      new_cache = true;
    }

  m_renderer->update_cache_items (m_container->root (), m_first_id_to_change, m_last_id_to_change, m_last_transform, new_cache);
  if (new_cache)
    cache->zoom_level_changed (cur_zoom_x, cur_zoom_y);
}

void renderer_thread::small_wait ()
{
  sleep_ms (1);
}
