#ifndef RENDERED_ITEMS_CACHE_H
#define RENDERED_ITEMS_CACHE_H

#include <map>
#include <utility>
#include <string>

class SkBitmap;
class render_cache_id;
class QMutex;
class QTransform;
class QColor;
struct cache_bitmap_t;

class rendered_items_cache
{
  static const int m_block_pixel_size;

  std::map<render_cache_id, cache_bitmap_t> *m_cache;
  std::map<render_cache_id, cache_bitmap_t> *m_next_zoom_cache;
  std::map<int, std::string> m_selection_map;

  double m_zoom_x, m_zoom_y;

  QMutex *m_mutex;
  bool m_pending_changes;

  std::map<int, SkBitmap> m_current_screen_map;

public:
  rendered_items_cache ();
  ~rendered_items_cache ();

  static int block_pixel_size () { return m_block_pixel_size; }

  void add_bitmap (const render_cache_id &id, const SkBitmap &pixmap, bool add_to_new_cache);
  void invalidate (const render_cache_id &id);
  void invalidate (const render_cache_id &first, const render_cache_id &last);
  bool is_cached (const render_cache_id &id, bool next_cache) const;

  void zoom_level_changed (double zoom_x, double zoom_y);
  void clear_next_zoom_cache ();
  void clear ();
  void clear_selection_mapping ();

  void lock ();
  void unlock ();

  bool has_pending_changes ();
  bool same_zoom (const QTransform &transform);

  void add_selection_mapping (int id, const std::string &name);
  void remove_selection_mapping (int id);

  void set_current_screen (const SkBitmap &bitmap, int cache_object_id);
  SkBitmap get_current_screen (int cache_object_id) const; 

  /// call this functions only after calling lock
  std::string get_selection_name (int id) const;
  SkBitmap bitmap (const render_cache_id &id) const;
  bool is_valid (const render_cache_id &id) const;
  double zoom_x () const { return m_zoom_x; }
  double zoom_y () const { return m_zoom_y; }
  //************************************ 

  static QColor get_selection_color (int id);
  static int get_id_by_color (const QColor &color);
};

#endif // RENDERED_ITEMS_CACHE_H
