#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include <QObject>

class mouse_event_t;
class QPainter;
class QRect;
class QTransform;

class svg_painter;

class abstract_tool : public QObject
{
protected:
  svg_painter *m_painter;
public:
  abstract_tool (svg_painter *painter) { m_painter = painter; }
  virtual ~abstract_tool () {}

  virtual void activate () = 0;
  virtual void deactivate () = 0;

  virtual void configure () = 0;
  virtual void draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform) = 0;

  virtual bool mouse_event (const mouse_event_t &m_event) = 0;
};

#endif // ABSTRACT_TOOL_H
