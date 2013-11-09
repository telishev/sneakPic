#ifndef ABSTRACT_HANDLE_H
#define ABSTRACT_HANDLE_H

#include "renderer/renderable_item.h"

class QPointF;
class QPoint;
class QTransform;

class abstract_handle : public renderable_item
{
public:
  virtual ~abstract_handle () {}

  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const = 0;

  virtual void set_mouse_hovered (bool hovered) = 0;

  virtual bool start_drag (QPointF local_pos) = 0;
  virtual bool drag (QPointF local_pos) = 0;
  virtual bool end_drag (QPointF local_pos) = 0;
};

#endif // ABSTRACT_HANDLE_H
