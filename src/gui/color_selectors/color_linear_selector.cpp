#include "gui/color_selectors/color_linear_selector.h"

#include <QMouseEvent>
#include <QPainter>

static const int SLIDER_WIDTH = 2;
static const int COLOR_LINEAR_SELECTOR_DEFAULT_LENGTH = 175;
static const int COLOR_LINEAR_SELECTOR_DEFAULT_WIDTH = 22;
static const int BORDER_WIDTH = 1;

void color_linear_selector::paintEvent (QPaintEvent * /*event*/)
{
  QPainter painter (this);
  if (is_selector_type_alpha (m_type))
    draw_checkerboard (painter);

  draw_gradient (painter);
  draw_controller(painter);
  draw_border (painter);
}

color_linear_selector::color_linear_selector (QWidget *parent, Qt::Orientation orientation, color_single_selector_type type)
  : color_selector (parent)
{
  m_orientation = orientation;
  m_type = type;
  switch (m_orientation)
    {
    case Qt::Horizontal:
      setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
      break;
    case Qt::Vertical:
      setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
      break;
    }
}

int color_linear_selector::get_current_slider_pos ()
{
  int length = get_controlled_length ();
  int value = get_controlled_param_value ();
  int maximum = get_controlled_param_maximum ();
  switch (m_orientation)
    {
    case Qt::Horizontal:
      break;
    case Qt::Vertical:
      value = maximum - value;
      break;
    }
  return BORDER_WIDTH + value * length / maximum;
}

int color_linear_selector::get_pos (QPoint point)
{
  switch (m_orientation)
    {
    case Qt::Horizontal:
      return point.x () - BORDER_WIDTH;
      break;
    case Qt::Vertical:
      return get_controlled_length () - point.y () - BORDER_WIDTH;
      break;
    }
  return 0;
}

int color_linear_selector::get_controlled_length ()
{
  switch (m_orientation)
    {
    case Qt::Horizontal:
      return this->width () - 2 * BORDER_WIDTH - SLIDER_WIDTH;
      break;
    case Qt::Vertical:
      return this->height () - 2 * BORDER_WIDTH - SLIDER_WIDTH;
      ;
      break;
    }
  return 0;
}

int color_linear_selector::get_controlled_param_maximum ()
{
  return get_param_maximum_by_type (m_type);
}

void color_linear_selector::set_color_from_pos (QPoint pos)
{
  m_color = change_param_for_control (m_color, (int) ((get_controlled_param_maximum ()) * get_pos (pos) / get_controlled_length ()));
}

int color_linear_selector::get_controlled_param_value ()
{
  return get_param_value_by_type (m_type);
}

QColor color_linear_selector::change_param_for_control (QColor color, int value)
{
  return change_param_by_type (color, value, m_type);
}

QSize color_linear_selector::sizeHint () const
{
  switch (m_orientation)
    {
    case Qt::Horizontal:
      return QSize (COLOR_LINEAR_SELECTOR_DEFAULT_LENGTH, COLOR_LINEAR_SELECTOR_DEFAULT_WIDTH);
      break;
    case Qt::Vertical:
      return QSize (COLOR_LINEAR_SELECTOR_DEFAULT_WIDTH, COLOR_LINEAR_SELECTOR_DEFAULT_LENGTH);
      break;
    }
  return QSize ();
}

int color_linear_selector::get_needed_number_of_points_for_gradient ()
{
  return get_needed_number_of_points_for_gradient_by_type (m_type);
}

void color_linear_selector::mousePressEvent (QMouseEvent *event)
{
  m_drag_started = true;
  set_color_from_cursor_pos(event);
}

void color_linear_selector::mouseMoveEvent (QMouseEvent *event)
{
  if (m_drag_started)
    set_color_from_cursor_pos (event);
}

void color_linear_selector::mouseReleaseEvent (QMouseEvent *event)
{
  if (m_drag_started)
    {
      set_color_from_cursor_pos (event);
      m_drag_started = false;
      emit color_changing_finished (m_color);
    }
}

void color_linear_selector::set_color_from_cursor_pos (QMouseEvent *event)
{
  set_color_from_pos (event->pos ());
  emit color_changed_momentarily (m_color);
  this->update ();
}

void color_linear_selector::draw_gradient (QPainter &painter)
{
  QLinearGradient gradient;
  QGradientStop start, finish;
  switch (m_orientation)
    {
    case Qt::Horizontal:
      gradient.setStart (BORDER_WIDTH, this->height () / 2);
      gradient.setFinalStop (this->width () - BORDER_WIDTH, this->height () / 2);
      break;
    case Qt::Vertical:
      gradient.setStart (this->width () / 2, BORDER_WIDTH);
      gradient.setFinalStop (this->width () / 2, this->height () - BORDER_WIDTH);
      break;
    }


  int needed_number_of_points = get_needed_number_of_points_for_gradient ();
  int maximum = get_controlled_param_maximum ();
  for (int i = 0; i < needed_number_of_points; i++)
    {
      double fraction  = (double) i / (needed_number_of_points - 1);
      QColor color = change_param_for_control (do_color_preprocessing (m_color), (int) (fraction * maximum));
      switch (m_orientation)
        {
        case Qt::Horizontal:
          break;
        case Qt::Vertical:
          fraction = 1.0 - fraction;
          break;
        }
      gradient.setColorAt (fraction, color);
    }
  switch (m_orientation)
    {
    case Qt::Horizontal:
      painter.fillRect (QRect (BORDER_WIDTH, BORDER_WIDTH, width () - 2 * BORDER_WIDTH, height () - 2 * BORDER_WIDTH), gradient);
      break;
    case Qt::Vertical:
      painter.fillRect (QRect (BORDER_WIDTH, BORDER_WIDTH, width () - 2 * BORDER_WIDTH, height () - 2 * BORDER_WIDTH), gradient);
      break;
    }
}

void color_linear_selector::draw_controller (QPainter &painter)
{
  int slider_pos = get_current_slider_pos ();
  QRect controller_rect;
  switch (m_orientation)
    {
    case Qt::Horizontal:
      controller_rect = QRect (slider_pos, BORDER_WIDTH, SLIDER_WIDTH, this->height () - 2 * BORDER_WIDTH - 1);
      break;
    case Qt::Vertical:
      controller_rect = QRect (BORDER_WIDTH, slider_pos, this->width () - 2 * BORDER_WIDTH - 1, SLIDER_WIDTH);
      break;
    }

  painter.setPen ("Black");
  painter.setBrush (QBrush ("White"));
  painter.drawRect (controller_rect);
  painter.setBrush (QBrush ());
}

QColor color_linear_selector::do_color_preprocessing (QColor color)
{
  return do_color_preprocessing_by_type (color, m_type);
}




