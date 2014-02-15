#ifndef ABSTRACT_RENDERER_ITEM_H
#define ABSTRACT_RENDERER_ITEM_H
 
class QPainter;
class SkCanvas;
class renderer_state;
class renderer_items_container;
class renderer_config;
class SkPaint;

#include "renderer/renderable_item.h"

#include <QRectF>
#include <QTransform>
#include <string>
#include <vector>

class abstract_renderer_item : public renderable_item
{
protected:
  std::string m_name;
  renderer_items_container *m_container;
  vector<std::string> m_children;
  std::string m_parent;
  QTransform m_transform;
  int m_unique_id;
  bool m_bbox_valid;
public:
  abstract_renderer_item (const std::string &name);
  virtual ~abstract_renderer_item ();

  QRectF bounding_box () const;
  void update_bbox ();
  void invalidate_bbox ();

  QTransform transform () const { return m_transform; }
  void set_transform (const QTransform &transform) { m_transform = transform; }

  std::string name () const { return m_name; }

  void set_container (renderer_items_container *container) { m_container = container; }
  renderer_items_container *container () const { return m_container; }

  void push_back_child (const std::string &child);

  void set_unique_id (int id) { m_unique_id = id; }
  int unique_id () const { return m_unique_id; }

  void erase_child (const std::string &child);

  void set_parent (const std::string &parent) { m_parent = parent; }
  abstract_renderer_item *parent () const;

protected:
  bool configure_painter_for_selection (SkPaint &paint) const;
  virtual void update_bbox_impl () = 0;
  virtual QRectF bounding_box_impl () const = 0;
};

#endif // ABSTRACT_RENDERER_ITEM_H
