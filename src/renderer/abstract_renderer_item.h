#ifndef ABSTRACT_RENDERER_ITEM_H
#define ABSTRACT_RENDERER_ITEM_H
 
class QPainter;
class SkCanvas;
class renderer_state;
class renderer_items_container;
class renderer_config;

#include <QRectF>
#include <QTransform>
#include <string>
#include <vector>

class abstract_renderer_item
{
protected:
  std::string m_name;
  renderer_items_container *m_container;
  std::vector<std::string> m_children;
  QTransform m_transform;
public:
  abstract_renderer_item (const std::string &name);
  virtual ~abstract_renderer_item ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const = 0;
  virtual QRectF bounding_box () const = 0;
  virtual void update_bbox () = 0;

  QTransform transform () const { return m_transform; }
  void set_transform (const QTransform &transform) { m_transform = transform; }

  std::string name () const { return m_name; }
  void set_container (renderer_items_container *container) { m_container = container; }
  renderer_items_container *container () const { return m_container; }

  void push_back_child (const std::string &child);
};

#endif // ABSTRACT_RENDERER_ITEM_H
