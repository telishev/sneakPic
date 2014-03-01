#include "common/memory_deallocation.h"

#include "gui/color_selectors/color_rectangular_selector.h"

#include <QMouseEvent>
#include <QPainter>

static int COLOR_RECTANGULAR_SELECTOR_DEFAULT_SIZE = 175;
static int RECTANGULAR_SELECTOR_CIRCLE_RADIUS = 5;

color_rectangular_selector::color_rectangular_selector (QWidget *parent, color_single_selector_type type_x, color_single_selector_type type_y, flags flags_arg)
  : color_selector (parent)
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
  return QRect (1, 1, width () - 2, height () - 2);
}

void color_rectangular_selector::update_cached_gradient ()
{
  FREE (m_gradient_cached);
  m_gradient_cached = new QImage (size (), QImage::Format_ARGB32_Premultiplied);
  QPainter painter (m_gradient_cached);
  if (is_selector_type_alpha (m_controlled_types[0]) ||
      is_selector_type_alpha (m_controlled_types[1]))
    draw_checkerboard (painter);
  for (int j = 0; j < height () - 1; j++)
    {
      int n_points = get_needed_number_of_points_for_gradient_by_type (m_controlled_types[0]);
      QLinearGradient gradient;
      gradient.setStart (1, j + 1);
      gradient.setFinalStop (width () - 1, j + 1);
      for (int i = 0; i < n_points; i++)
        {
          QColor color = do_color_preprocessing_by_two_types (m_color, m_controlled_types[0], m_controlled_types[1]);
          color = set_param_by_x_type (color, (i * get_x_param_maximum ()) / (n_points - 1));
          color = set_param_by_y_type (color, (j * get_y_param_maximum ()) / (height () - 2));
          gradient.setColorAt ((double) i / n_points, color);
        }

      painter.fillRect (QRect (1, j + 1, width () - 1, 1), gradient);
    }
  m_size_cached = size ();
  m_cached_color = m_color;
}

void color_rectangular_selector::paintEvent (QPaintEvent * /*event*/)
{
  QColor color = set_param_by_x_type (m_cached_color, get_param_by_x_type ());
  color = set_param_by_y_type (m_cached_color, get_param_by_y_type ());
  setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Maximum);
  if (m_size_cached != size () || m_color != m_cached_color)
    update_cached_gradient ();

  QPainter painter(this);
  painter.drawImage (rect (), *m_gradient_cached);
  draw_border (painter);
  painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
  painter.setPen(QColor ("White"));
  painter.setClipRect (gradient_rect ());
  QPoint point;
  point.setX (1 + (width () - 2) * get_param_by_x_type () / get_x_param_maximum ());
  point.setY (1 + (height () - 2) * get_param_by_y_type () / get_y_param_maximum ());
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

QColor color_rectangular_selector::set_param_by_x_type (QColor color, int value)
{
  return change_param_by_type (color, (m_flags & flag::X_REVERSED) ? get_x_param_maximum () - value : value, m_controlled_types[0]);
}

QColor color_rectangular_selector::set_param_by_y_type (QColor color, int value)
{
  return change_param_by_type (color, (m_flags & flag::Y_REVERSED) ? value : get_y_param_maximum () - value, m_controlled_types[1]);
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
      emit color_changing_finished (m_color);
    }
}

void color_rectangular_selector::mousePressEvent (QMouseEvent *event)
{
  m_drag_started = true;
  set_color_from_pos (event->pos ());
}

void color_rectangular_selector::set_color_from_pos (QPoint pos)
{
  set_param_by_x_type (m_color, ((pos.x () - 1) * get_x_param_maximum ()) / (width () - 2));
  set_param_by_y_type (m_color, ((pos.y () - 1) * get_y_param_maximum ()) / (height () - 2));
  emit color_changed_momentarily (m_color);
  update ();
}
