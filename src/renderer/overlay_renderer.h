#ifndef OVERLAY_RENDERER_H
#define OVERLAY_RENDERER_H

class svg_renderer;
class renderer_items_container;
class svg_document;
class renderer_overlay_root;
class renderer_page;
class items_selection;
class rendered_items_cache;

class QPainter;
class QRect;
class QTransform;

#include <string>
#include <set>

enum class overlay_item_type;

class overlay_renderer
{
  svg_renderer *m_renderer;
  renderer_items_container *m_container;
  svg_document *m_document;
  renderer_overlay_root *m_root;
  renderer_page *m_page_item;
  rendered_items_cache *m_cache;
  std::set<std::string> m_selection;

  std::string m_current_item;
public:
  overlay_renderer (rendered_items_cache *cache);
  ~overlay_renderer ();

  void set_document (svg_document *document);
  void set_current_item (const std::string &id);
  void selection_changed (const items_selection *selection);

  std::string current_item () const { return m_current_item; }

  void draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform);
  void draw_page (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform);

private:
  std::string add_item (const std::string &name, overlay_item_type type);
};

#endif // OVERLAY_RENDERER_H
