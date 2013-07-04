#ifndef RENDERER_STATE_H
#define RENDERER_STATE_H

#include <QRectF>
#include <QTransform>

class renderer_state
{
  QRectF m_rect_to_draw;
  QTransform m_transform; 
public:

  QRectF rect () const { return m_rect_to_draw; }
  void set_rect (QRectF val) { m_rect_to_draw = val; }

  QTransform transform () const { return m_transform; }
  void set_transform (QTransform val) { m_transform = val; }
};

#endif // RENDERER_STATE_H
