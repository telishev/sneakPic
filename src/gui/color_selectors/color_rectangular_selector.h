#ifndef COLOR_RECTANGULAR_SELECTOR_H
#define COLOR_RECTANGULAR_SELECTOR_H

#include "gui/color_selectors/color_selector.h"

class color_rectangular_selector : public color_selector
{
  color_single_selector_type m_controlled_types [2];
  bool m_drag_started;
  QImage *m_gradient_cached;
  QColor m_cached_color;
  QSize m_size_cached;

public:
  enum flag
  {
    EMPTY = 0x00,
    X_REVERSED = 0x01,
    Y_REVERSED = 0x02,
  };

  Q_DECLARE_FLAGS (flags, flag)

private:
  flags m_flags;

public:
  color_rectangular_selector (QWidget *parent, color_single_selector_type type_x, color_single_selector_type type_y, QColor *controlled_color, flags flags_arg = flag::EMPTY);
  ~color_rectangular_selector ();
protected:
  virtual QSize sizeHint() const;
  virtual void paintEvent (QPaintEvent *event) override;

  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void color_changed_externally () override;

private:
  void set_color_from_pos( QPoint pos );
  int get_x_param_maximum ();
  int get_y_param_maximum ();
  void set_param_by_x_type (QColor *color, int value);
  void set_param_by_y_type (QColor *color, int value);
  int get_param_by_x_type ();
  int get_param_by_y_type ();
  void update_cached_gradient ();
  QRect gradient_rect ();
};

Q_DECLARE_OPERATORS_FOR_FLAGS (color_rectangular_selector::flags)

#endif // COLOR_RECTANGULAR_SELECTOR_H
