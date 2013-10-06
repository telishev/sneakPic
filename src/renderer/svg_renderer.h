#ifndef SVG_RENDERER_H
#define SVG_RENDERER_H

class renderable_item;
class rendered_items_cache;
class render_cache_id;
class events_queue;
class renderer_state;
class renderer_config;

class QPainter;
class QRectF;
class QTransform;
class QRect;

class SkCanvas;
class SkBitmap;

class svg_renderer
{
  rendered_items_cache *m_cache;
  events_queue *m_queue;
public:
  svg_renderer (rendered_items_cache *cache, events_queue *queue);
  ~svg_renderer ();

  void draw_item (const renderable_item *item, SkCanvas &canvas, const renderer_state &state, renderer_config &cfg);

  void update_cache_items (const renderable_item *item, const render_cache_id &first,
                           const render_cache_id &last, QTransform transform, renderer_config &cfg);

  rendered_items_cache *cache () const { return m_cache; }

  SkBitmap *draw_to_bitmap (const QRect &rect_to_draw, const QTransform &transform, const renderable_item *item);
  void draw_to_bitmap (const QRect &rect_to_draw, const QTransform &transform, const renderable_item *item, SkBitmap *bitmap);

  void update_drawing (const QTransform &transform, const QRectF &rect_to_update, int cache_object_type);

private:
  bool is_something_cached ( const render_cache_id &first, const render_cache_id &last, renderer_config &cfg);
  void update_cache_item (const renderable_item *item, const render_cache_id &cache_id, const QTransform &transform,
                          renderer_config &cfg, int total_x, int total_y);

  void update_cache_item_async (const renderable_item *item, const render_cache_id &cache_id, const QTransform &transform,
                                renderer_config &cfg);
};

#endif // SVG_RENDERER_H
