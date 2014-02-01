#ifndef PATH_BUILDER_H
#define PATH_BUILDER_H

class svg_path;
#include <QPointF>

class path_builder
{
  svg_path &m_dst_path;
  QPointF m_cur_position;
  QPointF m_prev_quad_c; bool m_prev_is_quad;
  QPointF m_prev_curve_c; bool m_prev_is_curve;
  bool m_new_subpath_pending;
public:
  path_builder (svg_path &dst_path);
  ~path_builder ();

  void close_subpath ();
  void move_to (QPointF dst, bool relative);
  void line_to (QPointF dst, bool relative);
  void hline_to (double x, bool relative);
  void vline_to (double y, bool relative);
  void quad_to (QPointF dst, QPointF control_point, bool relative, bool symmetrical = false);
  void quad_to_short (QPointF dst, bool relative);
  void curve_to (QPointF dst, QPointF control_point1, QPointF control_point2, bool relative, bool symmetrical = false);
  void curve_to_short (QPointF dst, QPointF control_point2, bool relative);
  void arc_to (QPointF dst, double rx, double ry, double x_axis_rotation, int large_arc_flag, int sweep_flag, bool relative);

  void set_curve_c (QPointF c);
  void check_new_subpath ();

private:
  void set_prev_quad_c (QPointF c);
  void set_prev_curve_c (QPointF c);
  void clear_prev ();
};

#endif // PATH_BUILDER_H
