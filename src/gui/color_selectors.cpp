#include "color_selectors.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPainter>

#include "gui/mouse_filter.h"
#include "gui/connection.h"

#include <stdlib.h>

static int SLIDER_WIDTH = 2;
static int COLOR_SCROLLER_DEFAULT_LENGTH = 175;
static int COLOR_SCROLLER_DEFAULT_WIDTH = 22;
static int COLOR_INDICATOR_DEFAULT_SIZE = 25;
static int BORDER_WIDTH = 1;
static int CHECKERBOARD_ELEMENT_SIZE = 5;

void color_scroller::paintEvent (QPaintEvent * /*event*/)
{
  QPainter painter (this);
  if (m_type == color_single_selector_type::ALPHA)
    draw_checkerboard (painter);

  draw_gradient (painter);
  draw_controller(painter);
  draw_border (painter);
}

color_scroller::color_scroller (QWidget *parent, Qt::Orientation orientation, color_single_selector_type type, QColor *controlled_color)
  : color_selector (parent, controlled_color)
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

int color_scroller::get_current_slider_pos ()
{
  int length = get_controlled_length ();
  int value = get_controlled_param_value ();
  int limit = get_controlled_param_limit ();
  return BORDER_WIDTH + value * length / (limit - 1);
}

int color_scroller::get_pos (QPoint point)
{
  switch (m_orientation)
    {
    case Qt::Horizontal:
      return point.x ();
      break;
    case Qt::Vertical:
      return point.y ();
      break;
    }
  return 0;
}

int color_scroller::get_controlled_length ()
{
  switch (m_orientation)
    {
    case Qt::Horizontal:
      return this->width () - SLIDER_WIDTH * 2 + 1 - 2 * BORDER_WIDTH;
      break;
    case Qt::Vertical:
      return this->height () - SLIDER_WIDTH * 2 + 1 - 2 * BORDER_WIDTH;
      break;
    }
  return 0;
}

int color_selector::get_param_value_by_type (color_single_selector_type type)
{
  int h, s, l, r, g, b, a;
  m_color->getHsl (&h, &s, &l);
  m_color->getRgb (&r, &g, &b, &a);
  switch (type)
    {
    case color_single_selector_type::HUE:
      return h;
    case color_single_selector_type::LIGHTNESS:
      return l;
    case color_single_selector_type::SATURATION:
      return s;
    case color_single_selector_type::RED:
      return r;
    case color_single_selector_type::GREEN:
      return g;
    case color_single_selector_type::BLUE:
      return b;
    case color_single_selector_type::ALPHA:
      return a;
    }
  return 0;
}

int color_scroller::get_controlled_param_value ()
{
  return get_param_value_by_type (m_type);
}

static int get_param_limit_by_type (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HUE:
      return 360;
    case color_single_selector_type::LIGHTNESS:
    case color_single_selector_type::SATURATION:
    case color_single_selector_type::RED:
    case color_single_selector_type::BLUE:
    case color_single_selector_type::GREEN:
    case color_single_selector_type::ALPHA:
      return 256;
    }
  return 0;
}

int color_scroller::get_controlled_param_limit ()
{
  return get_param_limit_by_type (m_type);
}

void color_scroller::set_color_form_pos (QPoint pos)
{
  set_controlled_param (m_color, (int) ((get_controlled_param_limit () - 1) * get_pos (pos) / get_controlled_length ()));
}

void color_selector::set_param_by_type (QColor *color, int value, color_single_selector_type type)
{
  int h, s, l, r, g, b, a;
  color->getHsl (&h, &s, &l);
  color->getRgb (&r, &g, &b, &a);

  if (value < 0)
    value = 0;

  if (value >= get_param_limit_by_type (type))
    value = get_param_limit_by_type (type) - 1;

  switch (type)
    {
    case color_single_selector_type::HUE:
      color->setHsl (value, s, l, a);
      break;
    case color_single_selector_type::LIGHTNESS:
      color->setHsl (h, s, value, a);
      break;
    case color_single_selector_type::SATURATION:
      color->setHsl (h, value, l, a);
      break;
    case color_single_selector_type::RED:
      color->setRgb (value, g, b, a);
      break;
    case color_single_selector_type::GREEN:
      color->setRgb (r, value, b, a);
      break;
    case color_single_selector_type::BLUE:
      color->setRgb (r, g, value, a);
      break;
    case color_single_selector_type::ALPHA:
      color->setRgb (r, g, b, value);
      break;
    }
}

void color_scroller::set_controlled_param (QColor *color, int value)
{
  set_param_by_type (color, value, m_type);
}

QSize color_scroller::sizeHint () const
{
  switch (m_orientation)
    {
    case Qt::Horizontal:
      return QSize (COLOR_SCROLLER_DEFAULT_LENGTH, COLOR_SCROLLER_DEFAULT_WIDTH);
      break;
    case Qt::Vertical:
      return QSize (COLOR_SCROLLER_DEFAULT_WIDTH, COLOR_SCROLLER_DEFAULT_LENGTH);
      break;
    }
  return QSize ();
}

int color_scroller::get_needed_number_of_points ()
{
  switch (m_type)
    {
    case color_single_selector_type::HUE:
      return 7;
      break;
    case color_single_selector_type::LIGHTNESS:
      return 3;
      break;
    case color_single_selector_type::SATURATION:
      return 2;
      break;
    case color_single_selector_type::RED:
    case color_single_selector_type::GREEN:
    case color_single_selector_type::BLUE:
    case color_single_selector_type::ALPHA:
      return 2;
    }
  return 0;
}

void color_scroller::mousePressEvent (QMouseEvent *event)
{
  m_drag_started = true;
  set_color_from_cursor_pos(event);
}

void color_scroller::mouseMoveEvent (QMouseEvent *event)
{
  if (m_drag_started)
    set_color_from_cursor_pos (event);
}

void color_scroller::mouseReleaseEvent (QMouseEvent *event)
{
  if (m_drag_started)
    {
      set_color_from_cursor_pos (event);
      m_drag_started = false;
    }
}

void color_scroller::set_color_from_cursor_pos (QMouseEvent *event)
{
  set_color_form_pos (event->pos ());
  emit color_changed_internally ();
  this->update ();
}

void color_scroller::draw_gradient (QPainter &painter)
{
  QLinearGradient gradient;
  QGradientStop start, finish;
  switch (m_orientation)
    {
    case Qt::Horizontal:
      gradient.setStart (0, this->height () / 2);
      gradient.setFinalStop (this->width (), this->height () / 2);
      break;
    case Qt::Vertical:
      gradient.setStart (0, this->height () / 2);
      gradient.setFinalStop (this->width (), this->height () / 2);
      break;
    }


  int needed_number_of_points = get_needed_number_of_points ();
  int maximum = get_controlled_param_limit () - 1;
  for (int i = 0; i < needed_number_of_points; i++)
    {
      QColor color = *m_color;
      do_color_preprocessing (color);
      double fraction  = (double) i / (needed_number_of_points - 1);
      set_controlled_param (&color, (int) (fraction * maximum));
      gradient.setColorAt (fraction, color);
    }
  switch (m_orientation)
    {
    case Qt::Horizontal:
      painter.fillRect (QRect (BORDER_WIDTH, BORDER_WIDTH, width () - 1 - BORDER_WIDTH, height () - 1 - BORDER_WIDTH), gradient);
      break;
    case Qt::Vertical:
      painter.fillRect (QRect (BORDER_WIDTH, BORDER_WIDTH, width () - 1 - BORDER_WIDTH, height () - 1 - BORDER_WIDTH), gradient);
      break;
    }
}

void color_scroller::draw_controller (QPainter &painter)
{
  int slider_pos = get_current_slider_pos ();
  QRect controller_rect;
  switch (m_orientation)
    {
    case Qt::Horizontal:
      controller_rect = QRect (slider_pos, BORDER_WIDTH, SLIDER_WIDTH, this->height () - 1 - 2 * BORDER_WIDTH);
      break;
    case Qt::Vertical:
      controller_rect = QRect (BORDER_WIDTH, slider_pos, this->width () - 1 - 2 * BORDER_WIDTH, SLIDER_WIDTH);
      break;
    }

  painter.setPen ("Black");
  painter.setBrush (QBrush ("White"));
  painter.drawRect (controller_rect);
  painter.setBrush (QBrush ());
}

void color_selector::draw_border (QPainter &painter)
{
  painter.setPen ("Grey");
  painter.drawRect (0, 0, width () - 1, height () - 1);
}

void color_scroller::do_color_preprocessing (QColor &color)
{
  switch (m_type)
    {
    case color_single_selector_type::HUE:
      color.setHsl (0, 255, 128); // In this case we need basically the same palette for every color (rainbow)
      break;
    case color_single_selector_type::LIGHTNESS:
    case color_single_selector_type::SATURATION:
    case color_single_selector_type::RED:
    case color_single_selector_type::GREEN:
    case color_single_selector_type::BLUE:
      color.setAlpha (255); // In this case we don't need to see changes in alpha component
      break;
    case color_single_selector_type::ALPHA:
      break;
    }
}

void color_selector::draw_checkerboard (QPainter &painter)
{
  painter.setPen (Qt::NoPen);
  for (int i = 0; i < width () / CHECKERBOARD_ELEMENT_SIZE + 1; i++)
    for (int j = 0; j < height () / CHECKERBOARD_ELEMENT_SIZE + 1; j++)
      painter.fillRect (QRect (i * CHECKERBOARD_ELEMENT_SIZE + BORDER_WIDTH, j * CHECKERBOARD_ELEMENT_SIZE + BORDER_WIDTH,
                               CHECKERBOARD_ELEMENT_SIZE, CHECKERBOARD_ELEMENT_SIZE), (i + j) % 2 == 0 ? QBrush ("White") : QBrush ("LightGrey"));
}

void color_selector::color_changed_externally ()
{
  // In simple case only update / i.e. redraw need to be called
  update ();
}

QSize color_indicator::sizeHint () const
{
  return QSize (COLOR_INDICATOR_DEFAULT_SIZE, COLOR_INDICATOR_DEFAULT_SIZE);
}

void color_indicator::paintEvent (QPaintEvent * /*event*/)
{
  QPainter painter (this);
  draw_checkerboard (painter);
  painter.setBrush (QBrush (*m_color));
  painter.drawRect (this->rect ());
  draw_border (painter);
}

color_indicator::color_indicator (QWidget *parent, QColor *color)
  : color_selector (parent, color)
{
  setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
}

color_spinbox::color_spinbox (QWidget *parent, color_single_selector_type type, QColor *color) : color_selector (parent, color)
{
  m_layout = new QHBoxLayout ();
  m_type = type;
  this->setLayout (m_layout);
  m_spin_box = new QSpinBox ();
  m_spin_box->setMaximum (get_param_limit_by_type (m_type) - 1);
  m_spin_box->setMinimum (0);
  m_spin_box->setValue (get_param_value_by_type (m_type));
  m_spin_box->setSingleStep (1);
  m_layout->addWidget (m_spin_box);
  m_layout->setContentsMargins (0, 0, 0, 0);
  m_value_changed_connection = CONNECT (m_spin_box, SIGNAL (valueChanged (int)), this, SLOT (spinbox_value_changed (int)));
}

void color_spinbox::color_changed_externally ()
{
  TEMPORARY_DISCONNECT (m_value_changed_connection);
  m_spin_box->setValue (get_param_value_by_type (m_type));
}

void color_spinbox::spinbox_value_changed (int new_value)
{
  set_param_by_type (m_color, new_value, m_type);
  emit color_changed_internally ();
}
