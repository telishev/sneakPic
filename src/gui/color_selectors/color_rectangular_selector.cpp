#include "common/memory_deallocation.h"

#include "gui/color_selectors/color_rectangular_selector.h"

#include <QMouseEvent>
#include <QPainter>

static int COLOR_RECTANGULAR_SELECTOR_DEFAULT_SIZE = 175;
static int RECTANGULAR_SELECTOR_CIRCLE_RADIUS = 5;

color_rectangular_selector::color_rectangular_selector (QWidget *parent, color_single_selector_type type_x, color_single_selector_type type_y, QColor *controlled_color, flags flags_arg)
  : color_selector (parent, controlled_color)
{
  m_controlled_types [0] = type_x;
  m_controlled_types [1] = type_y;
  m_flags = flags_arg;
  m_gradient_cached = nullptr;
}

color_rectangular_selector::~color_rectangular_selector ()
{
  FREE (m_gradient_cached);
}

QSize color_rectangular_selector::sizeHint () const
{
  return QSize (COLOR_RECTANGULAR_SELECTOR_DEFAULT_SIZE, COLOR_RECTANGULAR_SELECTOR_DEFAULT_SIZE);

}

QRect color_rectangular_selector::gradient_rect ()
{
  return QRect (BORDER_WIDTH, BORDER_WIDTH, width () - 2 * BORDER_WIDTH, height () - 2 * BORDER_WIDTH);
}

void color_rectangular_selector::update_cached_gradient ()
{
  FREE (m_gradient_cached);
  m_gradient_cached = new QImage (size (), QImage::Format_ARGB32_Premultiplied);
  QPainter painter (m_gradient_cached);
  if (is_selector_type_alpha (m_controlled_types[0]) ||
      is_selector_type_alpha (m_controlled_types[1]))
    draw_checkerboard (painter);
  for (int j = 0; j < height () - 2 * BORDER_WIDTH + 1; j++)
    {
      int n_points = get_needed_number_of_points_for_gradient_by_type (m_controlled_types[0]);
      QLinearGradient gradient;
      gradient.setStart (BORDER_WIDTH, j + BORDER_WIDTH);
      gradient.setFinalStop (width () - BORDER_WIDTH, j + BORDER_WIDTH);
      for (int i = 0; i < n_points; i++)
        {
          QColor color = m_color ? *m_color : placeholder_color;
          do_color_preprocessing_by_two_types (color, m_controlled_types[0], m_controlled_types[1]);
          set_param_by_x_type (&color, (i * get_x_param_maximum ()) / (n_points - 1));
          set_param_by_y_type (&color, (j * get_y_param_maximum ()) / (height () - 2 * BORDER_WIDTH));
          gradient.setColorAt ((double) i / n_points, color);
        }

      painter.fillRect (QRect (BORDER_WIDTH, j + BORDER_WIDTH, width () - 2 * BORDER_WIDTH + 1, 1), gradient);
    }
  m_size_cached = size ();
  m_cached_color = m_color ? *m_color : placeholder_color;
}

void color_rectangular_selector::paintEvent (QPaintEvent * /*event*/)
{
  set_param_by_x_type (&m_cached_color, get_param_by_x_type ());
  set_param_by_y_type (&m_cached_color, get_param_by_y_type ());
  setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Maximum);
  if (m_size_cached != size () || (m_color ? *m_color : placeholder_color) != m_cached_color)
    update_cached_gradient ();

  QPainter painter(this);
  painter.drawImage (rect (), *m_gradient_cached);
  draw_border (painter);
  painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
  painter.setPen(QColor ("White"));
  painter.setClipRect (gradient_rect ());
  QPoint point;
  point.setX (BORDER_WIDTH + (width () - 2 * BORDER_WIDTH) * get_param_by_x_type () / get_x_param_maximum ());
  point.setY (BORDER_WIDTH + (height () - 2 * BORDER_WIDTH) * get_param_by_y_type () / get_y_param_maximum ());
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
      emit color_changing_finished ();
    }
}

void color_rectangular_selector::mousePressEvent (QMouseEvent *event)
{
  m_drag_started = true;
  set_color_from_pos (event->pos ());
}

void color_rectangular_selector::set_color_from_pos (QPoint pos)
{
  set_param_by_x_type (m_color, ((pos.x () - BORDER_WIDTH) * get_x_param_maximum ()) / (width () - BORDER_WIDTH * 2));
  set_param_by_y_type (m_color, ((pos.y () - BORDER_WIDTH) * get_y_param_maximum ()) / (height () - BORDER_WIDTH * 2));
  emit color_changed_momentarily ();
  update ();
}

void color_rectangular_selector::color_changed_externally ()
{
  update ();
}
