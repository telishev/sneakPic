#ifndef OVERLAY_RENDERER_H
#define OVERLAY_RENDERER_H

class svg_renderer;
class renderer_items_container;
class svg_document;
class renderer_overlay_root;
class renderer_page;
class items_selection;
class rendered_items_cache;
class abstract_renderer_item;
class overlay_items_container;

class QPainter;
class QRect;
class QTransform;

#include <string>
#include <set>

enum class overlay_item_type;

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
  svg_document *m_document;
  rendered_items_cache *m_base_cache;
  rendered_items_cache *m_overlay_cache;
  bool overlay_changed[overlay_layer_type::COUNT];

  renderer_items_container *m_container;
  abstract_renderer_item *m_root_items[overlay_layer_type::COUNT];

  std::set<overlay_items_container *> m_overlay_containers;
public:
  overlay_renderer (rendered_items_cache *cache);
  ~overlay_renderer ();

  void set_document (svg_document *document);

  void draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform);
  void draw_page (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform);

  renderer_items_container *container () const { return m_container; }
  svg_document *document () const { return m_document; }

  void add_overlay_item (overlay_layer_type type, abstract_renderer_item *item);
  void remove_overlay_item (overlay_layer_type type, const std::string &item);

  void add_overlay_container (overlay_items_container *container);
  void remove_overlay_container (overlay_items_container *container);

  void items_changed ();


private:
  std::string add_item (const std::string &name, overlay_item_type type);
  abstract_renderer_item *root (overlay_layer_type type) const;
  void remove_overlay_containers ();
};

#endif // OVERLAY_RENDERER_H
