#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include <QObject>

class mouse_event_t;
class QPainter;
class QRect;
class QTransform;

class abstract_tool : public QObject
{
public:
  virtual ~abstract_tool () {}

  virtual void activate () = 0;
  virtual void deactivate () = 0;

  virtual void items_changed () = 0;

  virtual void draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform) = 0;

  virtual bool mouse_event (const mouse_event_t &m_event) = 0;
};

#endif // ABSTRACT_TOOL_H