#ifndef COLOR_SELECTORS_H
#define COLOR_SELECTORS_H

#include <QWidget>

enum class color_single_selector_type
{
  RGB_RED,
  RGB_GREEN,
  RGB_BLUE,
  RGB_ALPHA,

  HSL_HUE,
  HSL_SATURATION,
  HSL_LIGHTNESS,
  HSL_ALPHA,

  HSV_HUE,
  HSV_SATURATION,
  HSV_VALUE,
  HSV_ALPHA,

  CMYK_CYAN,
  CMYK_MAGENTA,
  CMYK_YELLOW,
  CMYK_BLACK,
  CMYK_ALPHA
};

class QHBoxLayout;
class QRect;
class QSpinBox;

class color_selector : public QWidget
{
  Q_OBJECT
protected:
  QColor m_color;
public:
  color_selector (QWidget *parent);
  QColor color () const { return m_color; }

protected:
  void draw_checkerboard (QPainter &painter);
  void draw_border (QPainter &painter);
  int get_param_value_by_type (color_single_selector_type type);
  QColor change_param_by_type (QColor color, int value, color_single_selector_type type);
  int get_needed_number_of_points_for_gradient_by_type (color_single_selector_type type);
  QColor do_color_preprocessing_by_type (QColor color, color_single_selector_type type);
  QColor do_color_preprocessing_by_two_types (QColor color, color_single_selector_type first_type, color_single_selector_type second_type);
  int get_param_maximum_by_type (color_single_selector_type type);
  bool is_selector_type_alpha (color_single_selector_type type);
public slots:
  virtual void set_color (QColor color);
signals:
  void color_changed_momentarily (QColor color);
  void color_changing_finished (QColor color);
};
#endif // COLOR_SELECTORS_H
