#ifndef COLOR_LINEAR_SELECTOR_H
#define COLOR_LINEAR_SELECTOR_H

#include "gui/color_selectors/color_selector.h"

class color_linear_selector : public color_selector
{
  Q_OBJECT

  Qt::Orientation m_orientation;
  color_single_selector_type m_type;
  bool m_drag_started;

public:
  color_linear_selector (QWidget *parent, Qt::Orientation orientation, color_single_selector_type type);

protected:
  virtual QSize sizeHint() const;
  virtual void paintEvent (QPaintEvent *event) override;

  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;

private:
  int get_current_slider_pos ();
  int get_pos (QPoint point);
  int get_controlled_length ();
  int get_controlled_param_value ();
  int get_controlled_param_maximum ();
  void set_color_from_pos (QPoint pos);
  QColor change_param_for_control (QColor color, int value );
  void set_color_from_cursor_pos (QMouseEvent *event);
  void draw_gradient (QPainter &painter);
  void draw_controller (QPainter &painter);
  QColor do_color_preprocessing (QColor color);
  int get_needed_number_of_points_for_gradient();
};


#endif // COLOR_LINEAR_SELECTOR_H

