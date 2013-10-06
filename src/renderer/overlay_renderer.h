#ifndef OVERLAY_RENDERER_H
#define OVERLAY_RENDERER_H

class svg_renderer;
class renderer_page;
class rendered_items_cache;
class renderable_item;
class renderer_overlay_root;

class QPainter;
class QRect;
class QTransform;
class SkBitmap;
class SkCanvas;

#include <string>


enum class overlay_layer_type
{
  PAGE, ///< under svg image (for drawing page borders)
  BASE, ///< over svg image (for overlay items that changes not very often, uses the cache)
  TEMP, ///< over base overlay (for temporary items, don't use the cache)

  COUNT,
};

class overlay_renderer
{
  svg_renderer *m_renderer;
  rendered_items_cache *m_base_cache;
  rendered_items_cache *m_overlay_cache;

  renderer_overlay_root *m_items[(int)overlay_layer_type::COUNT];

public:
  overlay_renderer (rendered_items_cache *cache = nullptr);
  ~overlay_renderer ();

  void add_item (renderable_item *item, overlay_layer_type layer);
  void remove_item (renderable_item *item, overlay_layer_type layer);

  void draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform);
private:
  void render_layer (const QRect &rect_to_draw, const QTransform &transform, overlay_layer_type layer_type, SkBitmap &bitmap);
  void draw_base (SkCanvas &canvas);
};

#endif // OVERLAY_RENDERER_H
