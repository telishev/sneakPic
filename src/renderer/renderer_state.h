#ifndef RENDERER_STATE_H
#define RENDERER_STATE_H

#include <QRect>
#include <QTransform>

class renderer_state
{
  /// rect to draw in screen coordinates
  QRect m_rect_to_draw;
  /// current transform to screen coords
  QTransform m_transform; 
public:

  renderer_state (const QRect &rect, const QTransform &transform)
  {
    m_rect_to_draw = rect;
    m_transform = transform;
  }

  QRect rect () const { return m_rect_to_draw; }
  void set_rect (QRect val) { m_rect_to_draw = val; }

  QTransform transform () const { return m_transform; }
  void set_transform (QTransform val) { m_transform = val; }
};

#endif // RENDERER_STATE_H
