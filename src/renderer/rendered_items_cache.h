#ifndef RENDERED_ITEMS_CACHE_H
#define RENDERED_ITEMS_CACHE_H

#include <map>
#include <utility>

class SkBitmap;
class render_cache_id;
class QMutex;
class QTransform;

class rendered_items_cache
{
  static const int m_block_pixel_size;

  std::map<render_cache_id, SkBitmap> *m_cache;
  std::map<render_cache_id, SkBitmap> *m_next_zoom_cache;

  double m_zoom_x, m_zoom_y;

  QMutex *m_mutex;
  bool m_pending_changes;

public:
  rendered_items_cache ();
  ~rendered_items_cache ();

  static int block_pixel_size () { return m_block_pixel_size; }

  void add_bitmap (const render_cache_id &id, const SkBitmap &pixmap, bool add_to_new_cache);
  bool is_cached (const render_cache_id &id, bool next_cache) const;

  void zoom_level_changed (double zoom_x, double zoom_y);
  void clear_next_zoom_cache ();

  void lock ();
  void unlock ();

  bool has_pending_changes ();
  bool same_zoom (const QTransform &transform);

  /// call this functions only after calling lock
  SkBitmap bitmap (const render_cache_id &id) const;
  double zoom_x () const { return m_zoom_x; }
  double zoom_y () const { return m_zoom_y; }
  //************************************ 
};

#endif // RENDERED_ITEMS_CACHE_H
