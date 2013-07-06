#include "renderer/rendered_items_cache.h"

#include "renderer/render_cache_id.h"

#include <QPixmap>

const int rendered_items_cache::m_block_pixel_size = 256;

rendered_items_cache::rendered_items_cache ()
{

}

rendered_items_cache::~rendered_items_cache ()
{

}

QPixmap rendered_items_cache::pixmap (const render_cache_id &id) const
{
  auto it = m_cache.find (id);
  if (it == m_cache.end ())
    return QPixmap ();

  return it->second;
}

void rendered_items_cache::add_pixmap (const render_cache_id &id, const QPixmap &pixmap)
{
  m_cache[id] = pixmap;
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
