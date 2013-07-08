#include "renderer/rendered_items_cache.h"

#include "renderer/render_cache_id.h"

#pragma warning(push, 0)
#include <SkBitmap.h>
#pragma warning(pop)

const int rendered_items_cache::m_block_pixel_size = 256;

rendered_items_cache::rendered_items_cache ()
{

}

rendered_items_cache::~rendered_items_cache ()
{

}

SkBitmap rendered_items_cache::bitmap (const render_cache_id &id) const
{
  auto it = m_cache.find (id);
  if (it == m_cache.end ())
    return SkBitmap ();

  return it->second;
}

void rendered_items_cache::add_bitmap (const render_cache_id &id, const SkBitmap &bitmap)
{
  m_cache[id] = bitmap;
  ///TODO: cache cleanup
}

bool rendered_items_cache::is_cached (const render_cache_id &id) const
{
  return m_cache.find (id) != m_cache.end ();
}

void rendered_items_cache::zoom_level_changed ()
{
  m_cache.clear ();
}
