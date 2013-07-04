#ifndef ABSTRACT_RENDERER_ITEM_H
#define ABSTRACT_RENDERER_ITEM_H

class QPainter;
class svg_renderer;
class renderer_state;

#include <QRectF>
#include <QTransform>

class abstract_renderer_item
{
  QTransform m_transform;
public:
  virtual ~abstract_renderer_item () {};

  virtual void draw (svg_renderer &renderer, const renderer_state &state) const = 0;
  virtual QRectF bounding_box () const = 0;

  QTransform transform () const { return m_transform; }
  void set_transform (const QTransform &transform) { m_transform = transform; }

};

#endif // ABSTRACT_RENDERER_ITEM_H
