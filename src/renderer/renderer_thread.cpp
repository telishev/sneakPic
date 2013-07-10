#include "renderer/renderer_thread.h"

#include "common/wait_queue.h"
#include "common/memory_deallocation.h"
#include "common/math_defs.h"
#include "common/common_utils.h"

#include "renderer/abstract_renderer_event.h"
#include "renderer/svg_renderer.h"
#include "renderer/renderer_items_container.h"
#include "renderer/rendered_items_cache.h"

#include <QTransform>



renderer_thread::renderer_thread (svg_renderer *renderer, wait_queue<abstract_renderer_event> *queue, QObject *parent)
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
  while (!m_exit_needed)
    {
      m_queue->lock ();
      if (m_queue->empty_nolock ())
        {
          m_queue->timed_wait (m_wait_time);
        }
      else
        {
          abstract_renderer_event *ev = 0;
          m_queue->pull_front_nolock (&ev);
          while (ev)
            {
              m_queue->unlock ();
              ev->process (this);
              FREE (ev);
              small_wait ();

              m_queue->lock ();
              m_queue->pull_front_nolock (&ev);
            }

          
        }
      m_queue->unlock ();
      redraw ();
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
