#include "renderer/rendered_items_cache.h"

#include "common/memory_deallocation.h"

#include "renderer/render_cache_id.h"

#include <QMutex>

#pragma warning(push, 0)
#include <SkBitmap.h>

#pragma warning(pop)

const int rendered_items_cache::m_block_pixel_size = 256;

rendered_items_cache::rendered_items_cache ()
{
  m_mutex = new QMutex;
  m_cache = new std::map<render_cache_id, SkBitmap>;
  m_next_zoom_cache = new std::map<render_cache_id, SkBitmap>;
}

rendered_items_cache::~rendered_items_cache ()
{
  FREE (m_mutex);
  FREE (m_cache);
  FREE (m_next_zoom_cache);
}

SkBitmap rendered_items_cache::bitmap (const render_cache_id &id) const
{
  auto it = m_cache->find (id);
  if (it == m_cache->end ())
    return SkBitmap ();

  return it->second;
}

void rendered_items_cache::add_bitmap (const render_cache_id &id, const SkBitmap &bitmap, bool next_cache)
{
  QMutexLocker lock (m_mutex);
  auto cache_to_use = next_cache ? m_next_zoom_cache : m_cache;
  (*cache_to_use)[id] = bitmap;
  m_pending_changes = true;
  ///TODO: cache cleanup
}

bool rendered_items_cache::is_cached (const render_cache_id &id, bool next_cache) const
{
  QMutexLocker lock (m_mutex);
  auto cache_to_use = next_cache ? m_next_zoom_cache : m_cache;
  return cache_to_use->find (id) != cache_to_use->end ();
}

void rendered_items_cache::zoom_level_changed (double zoom_x, double zoom_y)
{
  QMutexLocker lock (m_mutex);
  std::swap (m_cache, m_next_zoom_cache);
  m_next_zoom_cache->clear ();
  m_zoom_x = zoom_x;
  m_zoom_y = zoom_y;
}

void rendered_items_cache::lock ()
{
  m_mutex->lock ();
}

void rendered_items_cache::unlock ()
{
  m_mutex->unlock ();
}

bool rendered_items_cache::has_pending_changes ()
{
  QMutexLocker lock (m_mutex);
  bool has_changes = m_pending_changes;
  m_pending_changes = false;
  return has_changes;
}
