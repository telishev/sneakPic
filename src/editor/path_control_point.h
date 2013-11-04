#ifndef PATH_CONTROL_POINT_H
#define PATH_CONTROL_POINT_H

#include "editor/abstract_handle.h"
#include <QPointF>


class svg_item_path;
class QRect;
class svg_path;
class svg_painter;

struct single_path_element;

class path_control_point : public abstract_handle
{
  svg_item_path *m_item;
  svg_painter *m_painter;
  int m_control_point_id;

  QPointF m_drag_start;
  QPointF m_drag_cur;

public:
  path_control_point (svg_painter *painter, svg_item_path *item, int control_point_id);
  virtual ~path_control_point ();

protected:
  virtual bool is_mouse_inside (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos) override;
  virtual bool drag (QPointF local_pos) override;
  virtual bool end_drag (QPointF local_pos) override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

private:
  QPointF get_handle_center () const;
  QRect get_element_rect (QTransform transform) const;

  void apply_drag ();
};

#endif // PATH_CONTROL_POINT_H
