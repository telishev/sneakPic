#include "color_selectors.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPainter>

#include "common/memory_deallocation.h"

#include "gui/mouse_filter.h"
#include "gui/connection.h"

#include <stdlib.h>

static int SLIDER_WIDTH = 2;
static int COLOR_LINEAR_SELECTOR_DEFAULT_LENGTH = 175;
static int COLOR_LINEAR_SELECTOR_DEFAULT_WIDTH = 22;
static int COLOR_RECTANGULAR_SELECTOR_DEFAULT_SIZE = 175;
static int RECTANGULAR_SELECTOR_CIRCLE_RADIUS = 5;
static int COLOR_INDICATOR_DEFAULT_SIZE = 25;
static int BORDER_WIDTH = 1;
static int CHECKERBOARD_ELEMENT_SIZE = 5;
#define HQ_BILINEAR_GRADIENT

void color_linear_selector::paintEvent (QPaintEvent * /*event*/)
{
  QPainter painter (this);
  if (m_type == color_single_selector_type::ALPHA)
    draw_checkerboard (painter);

  draw_gradient (painter);
  draw_controller(painter);
  draw_border (painter);
}

color_linear_selector::color_linear_selector (QWidget *parent, Qt::Orientation orientation, color_single_selector_type type, QColor *controlled_color)
  : color_selector (parent, controlled_color)
{
  m_orientation = orientation;
  m_type = type;
  switch (m_orientation)
    {
    case Qt::Horizontal:
      setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
      break;
    case Qt::Vertical:
      setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Maximum);
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
      return point.x ();
      break;
    case Qt::Vertical:
      return get_controlled_length () - point.y ();
      break;
    }
  return 0;
}

int color_linear_selector::get_controlled_length ()
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
  int h_hsl, h_hsv, s_hsl, s_hsv, v, l, r, g, b, a, c, m, y, k;
  m_color->getHsv (&h_hsv, &s_hsv, &v);
  m_color->getHsl (&h_hsl, &s_hsl, &l);
  m_color->getRgb (&r, &g, &b, &a);
  m_color->getCmyk (&c, &m, &y, &k);

  switch (type)
    {
    case color_single_selector_type::HUE_HSL:
      return h_hsl;
    case color_single_selector_type::HUE_HSV:
      return h_hsv;
    case color_single_selector_type::LIGHTNESS:
      return l;
    case color_single_selector_type::SATURATION_HSL:
      return s_hsl;
    case color_single_selector_type::SATURATION_HSV:
      return s_hsv;
    case color_single_selector_type::RED:
      return r;
    case color_single_selector_type::GREEN:
      return g;
    case color_single_selector_type::BLUE:
      return b;
    case color_single_selector_type::ALPHA:
      return a;
    case color_single_selector_type::CYAN:
      return c;
    case color_single_selector_type::MAGENTA:
      return m;
    case color_single_selector_type::YELLOW:
      return y;
    case color_single_selector_type::BLACK:
      return k;
    case color_single_selector_type::VALUE:
      return v;
    }
  return 0;
}

int color_linear_selector::get_controlled_param_value ()
{
  return get_param_value_by_type (m_type);
}

static int get_param_maximum_by_type (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HUE_HSL:
    case color_single_selector_type::HUE_HSV:
      return 359;
    case color_single_selector_type::LIGHTNESS:
    case color_single_selector_type::SATURATION_HSL:
    case color_single_selector_type::SATURATION_HSV:
    case color_single_selector_type::RED:
    case color_single_selector_type::BLUE:
    case color_single_selector_type::GREEN:
    case color_single_selector_type::ALPHA:
    case color_single_selector_type::CYAN:
    case color_single_selector_type::MAGENTA:
    case color_single_selector_type::BLACK:
    case color_single_selector_type::YELLOW:
    case color_single_selector_type::VALUE:
      return 255;
    }
  return 0;
}

int color_linear_selector::get_controlled_param_maximum ()
{
  return get_param_maximum_by_type (m_type);
}

void color_linear_selector::set_color_from_pos (QPoint pos)
{
  set_controlled_param (m_color, (int) ((get_controlled_param_maximum ()) * get_pos (pos) / get_controlled_length ()));
}

void color_selector::set_param_by_type (QColor *color, int value, color_single_selector_type type)
{
  int h, s_hsl, s_hsv, v, l, r, g, b, a, c, m, y, k;
  color->getHsl (&h, &s_hsl, &l);
  color->getRgb (&r, &g, &b, &a);
  color->getHsv (&h, &s_hsv, &v);
  color->getCmyk (&c, &m, &y, &k);

  if (value < 0)
    value = 0;

  if (value > get_param_maximum_by_type (type))
    value = get_param_maximum_by_type (type);

  switch (type)
    {
    case color_single_selector_type::HUE_HSL:
      color->setHsl (value, s_hsl, l, a);
      break;
    case color_single_selector_type::HUE_HSV:
      color->setHsv (value, s_hsv, v, a);
      break;
    case color_single_selector_type::LIGHTNESS:
      color->setHsl (h, s_hsl, value, a);
      break;
    case color_single_selector_type::SATURATION_HSL:
      color->setHsl (h, value, l, a);
      break;
    case color_single_selector_type::SATURATION_HSV:
      color->setHsv (h, value, v, a);
      break;
    case color_single_selector_type::VALUE:
      color->setHsv (h, s_hsv, value, a);
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
    case color_single_selector_type::CYAN:
      color->setCmyk (value, m, y, k);
      break;
    case color_single_selector_type::MAGENTA:
      color->setCmyk (c, value, y, k);
      break;
    case color_single_selector_type::YELLOW:
      color->setCmyk (c, m, value, k);
      break;
    case color_single_selector_type::BLACK:
      color->setCmyk (c, m, y, value);
      break;
    }
}

void color_linear_selector::set_controlled_param (QColor *color, int value)
{
  set_param_by_type (color, value, m_type);
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

int color_selector::get_needed_number_of_points_for_gradient_by_type (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HUE_HSL:
    case color_single_selector_type::HUE_HSV:
      return 7;
      break;
    case color_single_selector_type::LIGHTNESS:
      return 3;
      break;
    case color_single_selector_type::SATURATION_HSL:
    case color_single_selector_type::SATURATION_HSV:
    case color_single_selector_type::VALUE:
    case color_single_selector_type::RED:
    case color_single_selector_type::GREEN:
    case color_single_selector_type::BLUE:
    case color_single_selector_type::ALPHA:
    case color_single_selector_type::CYAN:
    case color_single_selector_type::MAGENTA:
    case color_single_selector_type::YELLOW:
    case color_single_selector_type::BLACK:
      return 2;
    }
  return 0;
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
    }
}

void color_linear_selector::set_color_from_cursor_pos (QMouseEvent *event)
{
  set_color_from_pos (event->pos ());
  emit color_changed_internally ();
  this->update ();
}

void color_linear_selector::draw_gradient (QPainter &painter)
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
      gradient.setStart (this->width () / 2, 0);
      gradient.setFinalStop (this->width () / 2, this->height ());
      break;
    }


  int needed_number_of_points = get_needed_number_of_points_for_gradient ();
  int maximum = get_controlled_param_maximum ();
  for (int i = 0; i < needed_number_of_points; i++)
    {
      QColor color = *m_color;
      do_color_preprocessing (color);
      double fraction  = (double) i / (needed_number_of_points - 1);
      set_controlled_param (&color, (int) (fraction * maximum));
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
      painter.fillRect (QRect (BORDER_WIDTH, BORDER_WIDTH, width () - 1 - BORDER_WIDTH, height () - 1 - BORDER_WIDTH), gradient);
      break;
    case Qt::Vertical:
      painter.fillRect (QRect (BORDER_WIDTH, BORDER_WIDTH, width () - 1 - BORDER_WIDTH, height () - 1 - BORDER_WIDTH), gradient);
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

void color_selector::do_color_preprocessing_by_type (QColor &color, color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HUE_HSL:
    case color_single_selector_type::HUE_HSV:
      color.setHsl (0, 255, 128); // In this case we need basically the same palette for every color (rainbow)
      break;
    case color_single_selector_type::LIGHTNESS:
    case color_single_selector_type::SATURATION_HSL:
    case color_single_selector_type::SATURATION_HSV:
    case color_single_selector_type::VALUE:
    case color_single_selector_type::CYAN:
    case color_single_selector_type::MAGENTA:
    case color_single_selector_type::YELLOW:
    case color_single_selector_type::BLACK:
    case color_single_selector_type::RED:
    case color_single_selector_type::GREEN:
    case color_single_selector_type::BLUE:
      color.setAlpha (255); // In this case we don't need to see changes in alpha component
      break;
    case color_single_selector_type::ALPHA:
      break;
    }
}

void color_selector::do_color_preprocessing_by_two_types (QColor &color, color_single_selector_type first_type, color_single_selector_type second_type)
{
  if (first_type != color_single_selector_type::ALPHA && second_type != color_single_selector_type::ALPHA)
    color.setAlpha (255);
}

void color_linear_selector::do_color_preprocessing (QColor &color)
{
  do_color_preprocessing_by_type (color, m_type);
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
  m_spin_box->setMaximum (get_param_maximum_by_type (m_type));
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

color_rectangular_selector::color_rectangular_selector (QWidget *parent, color_single_selector_type type_x, color_single_selector_type type_y, QColor *controlled_color, flags flags_arg)
  : color_selector (parent, controlled_color)
{
  m_controlled_types [0] = type_x;
  m_controlled_types [1] = type_y;
  m_flags = flags_arg;
  m_gradient_cached = 0;
}

color_rectangular_selector::~color_rectangular_selector ()
{
  FREE (m_gradient_cached);
}

QSize color_rectangular_selector::sizeHint () const
{
  return QSize (COLOR_RECTANGULAR_SELECTOR_DEFAULT_SIZE, COLOR_RECTANGULAR_SELECTOR_DEFAULT_SIZE);

}

void color_rectangular_selector::update_cached_gradient ()
{
  FREE (m_gradient_cached);
  m_gradient_cached = new QImage (size (), QImage::Format_ARGB32_Premultiplied);
  QPainter painter (m_gradient_cached);
  if (m_controlled_types[0] == color_single_selector_type::ALPHA ||
      m_controlled_types[1] == color_single_selector_type::ALPHA)
    draw_checkerboard (painter);
#ifndef HQ_BILINEAR_GRADIENT
  int small_image_width = get_needed_number_of_points_for_gradient_by_type (m_controlled_types[0]) * 32;
  int small_image_height = get_needed_number_of_points_for_gradient_by_type (m_controlled_types[1]) * 32;
  QImage image (small_image_width, small_image_height, QImage::Format_ARGB32_Premultiplied);
  QImage alpha = image.alphaChannel ();

  for (int j = 0; j < small_image_height; j++)
    {
      QRgb *color_scanline = (QRgb *) image.scanLine (j);
      uchar *alpha_scanline = alpha.scanLine (j);
      for (int i = 0; i < small_image_width; i++)
        {
          QColor color = *m_color;
          do_color_preprocessing_by_two_types (color, m_controlled_types[0], m_controlled_types[1]);
          set_param_by_x_type (&color, (i * get_x_param_maximum ()) / (small_image_width - 1));
          set_param_by_y_type (&color, (j * get_y_param_maximum ()) / (small_image_height - 1));
          color_scanline[i] = color.rgb ();
          alpha_scanline[i] = color.alpha ();
        }
    }
  image.setAlphaChannel (alpha);
  painter.setRenderHint (QPainter::SmoothPixmapTransform, true);
  painter.drawImage (rect (), image, image.rect ());
#else // HQ_BILINEAR_GRADIENT
  for (int j = 0; j < height (); j++)
    {
      int n_points = get_needed_number_of_points_for_gradient_by_type (m_controlled_types[0]);
      QLinearGradient gradient;
      gradient.setStart (0, j);
      gradient.setFinalStop (width (), j);
      for (int i = 0; i < n_points; i++)
        {
          QColor color = *m_color;
          do_color_preprocessing_by_two_types (color, m_controlled_types[0], m_controlled_types[1]);
          set_param_by_x_type (&color, (i * get_x_param_maximum ()) / (n_points - 1));
          set_param_by_y_type (&color, (j * get_y_param_maximum ()) / (height () - 1));
          gradient.setColorAt ((double) i / n_points, color);
        }

      painter.fillRect (QRect (0, j, width (), 1), gradient);
    }
#endif // HQ_BILINEAR_GRADIENT
  m_size_cached = size ();
  m_cached_color = *m_color;
}

void color_rectangular_selector::paintEvent (QPaintEvent * /*event*/)
{
  set_param_by_x_type (&m_cached_color, get_param_by_x_type ());
  set_param_by_y_type (&m_cached_color, get_param_by_y_type ());
  setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Maximum);
  if (m_size_cached != size () || *m_color != m_cached_color)
    update_cached_gradient ();

  QPainter painter(this);
  painter.drawImage (rect (), *m_gradient_cached);
  painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
  painter.setPen(QColor ("White"));
  QPoint point;
  point.setX (width () * get_param_by_x_type () / get_x_param_maximum ());
  point.setY (height () * get_param_by_y_type () / get_y_param_maximum ());
  painter.drawEllipse (point, RECTANGULAR_SELECTOR_CIRCLE_RADIUS, RECTANGULAR_SELECTOR_CIRCLE_RADIUS);
}

int color_rectangular_selector::get_x_param_maximum ()
{
  return get_param_maximum_by_type (m_controlled_types[0]);
}

int color_rectangular_selector::get_y_param_maximum ()
{
  return get_param_maximum_by_type (m_controlled_types[1]);
}

void color_rectangular_selector::set_param_by_x_type (QColor *color, int value)
{
  set_param_by_type (color, (m_flags & flag::X_REVERSED) ? get_x_param_maximum () - value : value, m_controlled_types[0]);
}

void color_rectangular_selector::set_param_by_y_type (QColor *color, int value)
{
  set_param_by_type (color, (m_flags & flag::Y_REVERSED) ? value : get_y_param_maximum () - value, m_controlled_types[1]);
}

int color_rectangular_selector::get_param_by_x_type ()
{
  int value = get_param_value_by_type (m_controlled_types[0]);
  return (m_flags & flag::X_REVERSED) ? get_x_param_maximum () - value : value;
}

int color_rectangular_selector::get_param_by_y_type ()
{
  int value = get_param_value_by_type (m_controlled_types[1]);
  return (m_flags & flag::Y_REVERSED) ? value : get_y_param_maximum () - value;
}

void color_rectangular_selector::mouseMoveEvent (QMouseEvent *event)
{
  if (m_drag_started)
    {
      set_color_from_pos (event->pos ());
    }
}

void color_rectangular_selector::mouseReleaseEvent (QMouseEvent *event)
{
  if (m_drag_started)
    {
      set_color_from_pos (event->pos ());
      m_drag_started = false;
    }
}

void color_rectangular_selector::mousePressEvent (QMouseEvent *event)
{
  m_drag_started = true;
  set_color_from_pos (event->pos ());
}

void color_rectangular_selector::set_color_from_pos (QPoint pos)
{
  emit color_changed_internally ();
  set_param_by_x_type (m_color, (pos.x () * get_x_param_maximum ()) / width ());
  set_param_by_y_type (m_color, (pos.y () * get_y_param_maximum ()) / height ());
  update ();
}

void color_rectangular_selector::color_changed_externally ()
{
  update ();
}

