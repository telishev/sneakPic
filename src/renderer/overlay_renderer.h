#ifndef OVERLAY_RENDERER_H
#define OVERLAY_RENDERER_H

class svg_renderer;
class renderer_items_container;
class svg_document;
class renderer_overlay_root;

class QPainter;
class QRect;
class QTransform;

#include <string>

class overlay_renderer
{
  svg_renderer *m_renderer;
  renderer_items_container *m_container;
  svg_document *m_document;
  renderer_overlay_root *m_root;

  std::string m_current_item;
public:
  overlay_renderer ();
  ~overlay_renderer ();

  void set_document (svg_document *document);
  void set_current_item (const std::string &id);

  std::string current_item () const { return m_current_item; }

  void draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform);

};

#endif // OVERLAY_RENDERER_H
