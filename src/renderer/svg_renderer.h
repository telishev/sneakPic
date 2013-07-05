#ifndef SVG_RENDERER_H
#define SVG_RENDERER_H

class abstract_svg_item;
class rendered_items_cache;
class render_cache_id;

class QPainter;
class QRectF;
class QTransform;

class svg_renderer
{
  rendered_items_cache *m_cache;
public:
  svg_renderer (rendered_items_cache *cache);
  ~svg_renderer ();

  void draw_item (const abstract_svg_item *item, QPainter &painter, const QRectF &rect_to_draw, const QTransform &transform);
  void update_cache_item (const abstract_svg_item *item, const render_cache_id &cache_id, const QTransform &transform);
};

#endif // SVG_RENDERER_H
