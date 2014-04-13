#ifndef ABSTRACT_HANDLE_H
#define ABSTRACT_HANDLE_H

#include "renderer/renderable_item.h"

class QPointF;
class QPoint;
class QTransform;

enum class handle_priority
{
  NORMAL,
  LOW,
};

class abstract_handle : public renderable_item
{
public:
  static const int max_distance = 10;

  virtual ~abstract_handle () {}

  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const = 0;

  virtual void set_mouse_hovered (bool hovered) = 0;

  virtual bool start_drag (QPointF local_pos, QTransform transform) = 0;
  virtual bool drag (QPointF local_pos, QTransform transform) = 0;
  virtual bool end_drag (QPointF local_pos, QTransform transform) = 0;
  virtual void interrupt_drag () = 0;
  virtual handle_priority priority () const { return handle_priority::NORMAL; }

};

#endif // ABSTRACT_HANDLE_H
