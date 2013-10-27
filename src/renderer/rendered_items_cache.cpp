#include "renderer/rendered_items_cache.h"

#include "common/memory_deallocation.h"
#include "common/math_defs.h"
#include "common/debug_utils.h"

#include "renderer/render_cache_id.h"

#include <QMutex>
#include <QTransform>
#include <QColor>

#pragma warning(push, 0)
#include <SkBitmap.h>
#include <set>
#pragma warning(pop)

const int rendered_items_cache::m_block_pixel_size = 256;

rendered_items_cache::rendered_items_cache ()
{
  m_mutex = new QMutex;
  m_cache = new std::map<render_cache_id, SkBitmap>;
  m_next_zoom_cache = new std::map<render_cache_id, SkBitmap>;
  m_pending_changes = false;
  m_zoom_y = m_zoom_x = 1.0;
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
  DEBUG_ASSERT (id.object_type () != (int)render_cache_type::INVALID);
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

void rendered_items_cache::clear_next_zoom_cache ()
{
  QMutexLocker lock (m_mutex);
  m_next_zoom_cache->clear ();
}

bool rendered_items_cache::same_zoom (const QTransform &transform)
{
  QMutexLocker lock (m_mutex);
  double cache_zoom_x = zoom_x ();
  double cache_zoom_y = zoom_y ();
  double cur_zoom_x = transform.m11 ();
  double cur_zoom_y = transform.m22 ();
  if (are_equal (cache_zoom_x, cur_zoom_x) && are_equal (cache_zoom_y, cur_zoom_y))
    return true;
  else
    return false;
}

void rendered_items_cache::clear ()
{
  QMutexLocker lock (m_mutex);
  m_next_zoom_cache->clear ();
  m_cache->clear ();
}

void rendered_items_cache::add_selection_mapping (int id, const std::string &name)
{
  QMutexLocker lock (m_mutex);
  DEBUG_ASSERT (m_selection_map.find (id) == m_selection_map.end ());
  m_selection_map[id] = name;
}

void rendered_items_cache::remove_selection_mapping (int id)
{
  QMutexLocker lock (m_mutex);
  auto it = m_selection_map.find (id);
  DEBUG_ASSERT (it != m_selection_map.end ());
  m_selection_map.erase (it);
}

std::string rendered_items_cache::get_selection_name (int id) const
{
  auto it = m_selection_map.find (id);
  if (it != m_selection_map.end ())
    return it->second;

  return std::string ();
}

QColor rendered_items_cache::get_selection_color (int id)
{
  int r = (((id) >>  0) & 0xFF);
  int g = (((id) >>  8) & 0xFF);
  int b = (((id) >> 16) & 0xFF);
  int a = 0xFF;

  return QColor (r, g, b, a);
}

int rendered_items_cache::get_id_by_color (const QColor &color)
{
  int r = color.red ();
  int g = color.green ();
  int b = color.blue ();

  return (b << 16) | (g << 8) | (r << 0);
}

void rendered_items_cache::clear_selection_mapping ()
{
  QMutexLocker lock (m_mutex);
  m_selection_map.clear ();
}

void rendered_items_cache::remove_from_cache (const render_cache_id &id)
{
  QMutexLocker lock (m_mutex);
  m_cache->erase (id);
}

void rendered_items_cache::remove_from_cache (const render_cache_id &first, const render_cache_id &last)
{
  QMutexLocker lock (m_mutex);
  std::set<render_cache_id> items_to_remove;
  for (auto &item_pair : *m_cache)
    {
      render_cache_id cur_id = item_pair.first;
      if (cur_id.object_type () != first.object_type ())
        continue;;

      if (   cur_id.x () >= first.x () && cur_id.x () <= last.x ()
          && cur_id.y () >= first.y () && cur_id.y () <= last.y ())
        items_to_remove.insert (cur_id);
    }

  for (const auto &id : items_to_remove)
    m_cache->erase (id);
}

void rendered_items_cache::set_current_screen (const SkBitmap &bitmap, int cache_object_id)
{
  m_current_screen_map[cache_object_id] = bitmap;
}

SkBitmap rendered_items_cache::get_current_screen (int cache_object_id) const
{
  auto it = m_current_screen_map.find (cache_object_id);
  if (it == m_current_screen_map.end ())
    return SkBitmap ();

  return it->second;
}
