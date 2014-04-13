#ifndef GRADIENT_HANDLES_H
#define GRADIENT_HANDLES_H

#include "base_handle.h"

class gradient_handles;
class renderer_base_gradient_item;
class gradient_handles;
class renderer_linear_gradient;
class renderer_radial_gradient;

class base_gradient_handle : public base_handle
{
protected:
  QRectF m_bbox;
  renderer_base_gradient_item *m_gradient;
  int m_stop_num;
  QPointF m_drag_start;
  gradient_handles *m_handle;
public:
  base_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num);

  QColor current_color () const;

  void set_current_color (QColor color);
  std::pair<std::string, int> get_selection_id () const;
  
  virtual bool start_drag (QPointF local_pos, QTransform transform) override;
  virtual bool drag (QPointF local_pos, QTransform transform) override;
  virtual bool end_drag (QPointF local_pos, QTransform transform) override;
  virtual void interrupt_drag () override;
  virtual QPointF get_handle_center () const override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

  bool remove ();
  double distance_to_line (QPointF pos, QTransform cur_transform) const;
  void add_handle (QPointF local_pos);

protected:
  virtual QPointF start_point () const = 0;
  virtual QPointF end_point () const = 0;
  virtual void move_start_point (QPointF dst) = 0;
  virtual void move_end_point (QPointF dst) = 0;

protected:
  QPointF start_point_local () const;
  QPointF end_point_local () const;

  bool is_start () const;
  bool is_end () const;

  handle_type get_handle_type () const;
  QTransform gradient_transform () const;
  QPointF cur_point () const;
};

class linear_gradient_handle : public base_gradient_handle
{
public:
  linear_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num);

protected:
  virtual QPointF start_point () const;
  virtual QPointF end_point () const;
  virtual void move_start_point (QPointF dst);
  virtual void move_end_point (QPointF dst);
  renderer_linear_gradient *gradient () const;
};

class radial_gradient_handle : public base_gradient_handle
{
  bool m_x_axis;
public:
  radial_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num, bool is_x_axis);

protected:
  virtual QPointF start_point () const;
  virtual QPointF end_point () const;

  QPointF get_end_point (bool x_axis) const;

  virtual void move_start_point (QPointF dst);
  virtual void move_end_point (QPointF end_dst);
  renderer_radial_gradient *gradient () const;
};

#endif // GRADIENT_HANDLES_H
