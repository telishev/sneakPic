#ifndef RENDERED_ITEMS_CACHE_H
#define RENDERED_ITEMS_CACHE_H

#include <map>
#include <utility>

class QPixmap;
class render_cache_id;

class rendered_items_cache
{
  static const int m_block_pixel_size;
  double m_block_local_size;
  std::map<render_cache_id, QPixmap> m_cache;

public:
  rendered_items_cache ();
  ~rendered_items_cache ();

  QPixmap pixmap (const render_cache_id &id) const;
  void add_pixmap (const render_cache_id &id, const QPixmap &pixmap);
  bool is_cached (const render_cache_id &id) const;
  void zoom_level_changed ();

  static int block_pixel_size () { return m_block_pixel_size; }

};

#endif // RENDERED_ITEMS_CACHE_H
