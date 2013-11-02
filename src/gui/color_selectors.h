#ifndef COLOR_SELECTORS_H
#define COLOR_SELECTORS_H

#include <QWidget>

enum class color_single_selector_type
{
  HUE,
  LIGHTNESS,
  SATURATION,
  RED,
  GREEN,
  BLUE,
  ALPHA,
};

class QHBoxLayout;
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
public slots:
  virtual void color_changed_externally ();
signals:
  void color_changed_internally ();

};

class color_scroller : public color_selector
{
  Q_OBJECT

  Qt::Orientation m_orientation;
  color_single_selector_type m_type;
  QColor *m_controlled_color;
  bool m_drag_started;

public:
  color_scroller (QWidget *parent, Qt::Orientation orientation, color_single_selector_type type, QColor *controlled_color);

protected:
  virtual QSize sizeHint() const;
  virtual void paintEvent (QPaintEvent *event) override;

  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);

private:
  int get_current_slider_pos ();
  int get_pos (QPoint point);
  int get_controlled_length ();
  int get_controlled_param_value ();
  int get_controlled_param_limit ();
  void set_color_form_pos (QPoint pos);
  void set_controlled_param( QColor *color, int value );
  void set_color_from_cursor_pos (QMouseEvent *event);
  void draw_gradient (QPainter &painter);
  void draw_controller (QPainter &painter);
  void do_color_preprocessing( QColor &color );
  int get_needed_number_of_points();
};

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

public:
  color_spinbox (QWidget *parent, color_single_selector_type type, QColor *color);

protected slots:
  virtual void color_changed_externally();

private slots:
  void spinbox_value_changed (int new_value);
};


#endif // COLOR_SELECTORS_H
