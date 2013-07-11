#ifndef SVG_RENDERER_H
#define SVG_RENDERER_H

class abstract_renderer_item;
class rendered_items_cache;
class render_cache_id;
class events_queue;

class QPainter;
class QRectF;
class QTransform;

class SkCanvas;

class svg_renderer
{
  rendered_items_cache *m_cache;
  events_queue *m_queue;
public:
  svg_renderer (rendered_items_cache *cache, events_queue *queue);
  ~svg_renderer ();

  void draw_item (const abstract_renderer_item *item, SkCanvas &canvas, const QRectF &rect_to_draw, const QTransform &transform);

  void update_cache_items (const abstract_renderer_item *item, const render_cache_id &first,
                           const render_cache_id &last, QTransform transform, bool next_cache);

  rendered_items_cache *cache () const { return m_cache; }

private:
  bool is_something_cached ( const render_cache_id &first, const render_cache_id &last, bool next_cache);
  void update_cache_item (const abstract_renderer_item *item, const render_cache_id &cache_id, const QTransform &transform,
                          int total_x, int total_y, bool next_cache);
};

#endif // SVG_RENDERER_H
