#ifndef PATH_CONTROL_POINT_RENDERER_H
#define PATH_CONTROL_POINT_RENDERER_H

#include "renderer/renderable_item.h"

#include <QPointF>


class path_control_point_renderer : public renderable_item
{
  static const int handle_radius_px = 3;
  QPointF m_anchor, m_control_point;
  bool m_is_visible, m_is_highlighted;
public:
  path_control_point_renderer ();
  path_control_point_renderer (QPointF anchor, QPointF control_point, bool is_highlighted);
  ~path_control_point_renderer ();

  void set_anchor (QPointF anchor);
  void set_control_point (QPointF control_point);
  void set_visible (bool is_visible);
  void set_highlighted (bool is_highlighted);
  static int get_handle_radius () { return handle_radius_px; }

  QPointF anchor_pos () const { return m_anchor; }
  QPointF control_point () const { return m_control_point; }

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

private:
  QColor current_color () const;
  QColor line_color () const;

};

#endif // PATH_CONTROL_POINT_RENDERER_H
