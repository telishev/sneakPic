#ifndef COLOR_SELECTORS_H
#define COLOR_SELECTORS_H

#include <QWidget>

#include <memory>

class connection;

enum class color_single_selector_type
{
  HUE_HSL,
  LIGHTNESS,
  SATURATION_HSL,
  HUE_HSV,
  VALUE,
  SATURATION_HSV,
  RED,
  GREEN,
  BLUE,
  ALPHA,
  CYAN,
  MAGENTA,
  YELLOW,
  BLACK
};

class QHBoxLayout;
class QRect;
class QSpinBox;

class color_selector : public QWidget
{
  Q_OBJECT
protected:
  QColor *m_color;
public:
  color_selector (QWidget *parent, QColor *color) : QWidget (parent) { m_color = color;}
protected:
  void draw_checkerboard (QPainter &painter);
  void draw_border (QPainter &painter);
  int get_param_value_by_type (color_single_selector_type type);
  void set_param_by_type (QColor *color, int value, color_single_selector_type type);
  int get_needed_number_of_points_for_gradient_by_type (color_single_selector_type type);
  void do_color_preprocessing_by_type (QColor &color, color_single_selector_type type);
  void do_color_preprocessing_by_two_types (QColor &color, color_single_selector_type first_type, color_single_selector_type second_type);
public slots:
  virtual void color_changed_externally ();
signals:
  void color_changed_internally ();
};

class color_linear_selector : public color_selector
{
  Q_OBJECT

  Qt::Orientation m_orientation;
  color_single_selector_type m_type;
  QColor *m_controlled_color;
  bool m_drag_started;

public:
  color_linear_selector (QWidget *parent, Qt::Orientation orientation, color_single_selector_type type, QColor *controlled_color);

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
  void set_controlled_param( QColor *color, int value );
  void set_color_from_cursor_pos (QMouseEvent *event);
  void draw_gradient (QPainter &painter);
  void draw_controller (QPainter &painter);
  void do_color_preprocessing(QColor &color);
  int get_needed_number_of_points_for_gradient();
};

class color_rectangular_selector : public color_selector
{
  color_single_selector_type m_controlled_types [2];
  bool m_drag_started;
  QImage *m_gradient_cached;
  QColor m_cached_color;
  QSize m_size_cached;

public:
  enum class flag
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

};

Q_DECLARE_OPERATORS_FOR_FLAGS (color_rectangular_selector::flags)

class color_indicator : public color_selector
{
  Q_OBJECT

public:
  color_indicator (QWidget *parent, QColor *color);

protected:
  virtual QSize sizeHint() const;
  virtual void paintEvent (QPaintEvent *event) override;
};

class color_spinbox : public color_selector
{
  Q_OBJECT
  color_single_selector_type m_type;
  QHBoxLayout *m_layout;
  QSpinBox *m_spin_box;

  std::unique_ptr<connection> m_value_changed_connection;

public:
  color_spinbox (QWidget *parent, color_single_selector_type type, QColor *color);

protected slots:
  virtual void color_changed_externally();

private slots:
  void spinbox_value_changed (int new_value);
};


#endif // COLOR_SELECTORS_H
