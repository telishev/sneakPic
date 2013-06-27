/// \file  gui/mouse_filter.cpp
/// \brief This class eliminates small cursor positioning mistakes, e.g. 1-2 pixels accidental move when user presses mouse button.
///        All mouse events  should be provided in  this class by  class methods calling.  Any mouse state should be took from this
///        class by calling it's methods.



#include "gui/mouse_filter.h"

#include <stdlib.h>

#ifdef _WIN32
 #include <windows.h>
#else // !_WIN32
 #include <sys/time.h>
#endif // !_WIN32

#include <stdlib.h>

// Utility function.
static inline unsigned int get_time ()
{
#ifdef _WIN32
  return GetTickCount ();
#else // !_WIN32
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif // !_WIN32
}

// Utility function.
static inline
mouse_filter::mouse_button qt_mouse_button_2_mouse_filter_button (Qt::MouseButton qmousebutton)
{
  switch (qmousebutton)
    {
    case Qt::LeftButton:
      return mouse_filter::BUTTON_LEFT;
    case Qt::RightButton:
      return mouse_filter::BUTTON_RIGHT;
    case Qt::MidButton:
      return mouse_filter::BUTTON_MIDDLE;
    default:
      return mouse_filter::BUTTON_UNKNOWN;
    }

  return mouse_filter::BUTTON_UNKNOWN;
}



// Initializing Constructor.
mouse_filter::mouse_filter (QObject *parent, int _use_is_started_lr_click /*=0*/, int move_threshold /*= 2*/, unsigned int time_to_move /*= 400*/)
  : QObject (parent),
    move_threshold (move_threshold),
    time_to_move (time_to_move),
    use_is_started_lr_click (_use_is_started_lr_click)
{
  memset (pressed, 0, BUTTONS_COUNT * sizeof (unsigned char));
  memset (dragged, 0, BUTTONS_COUNT * sizeof (unsigned char));

  unsigned int i;
  for (i = 0; i < BUTTONS_COUNT; i++)
    start_point[i] = QPoint (-1, -1);

  memset (start_time, 0, BUTTONS_COUNT * sizeof (unsigned int));
  is_started_lr_click = 0;
}

// Destructor.
mouse_filter::~mouse_filter ()
{
  move_threshold = -1;
  time_to_move = (unsigned int) -1;

  memset (pressed, 0, BUTTONS_COUNT * sizeof (unsigned char));
  memset (dragged, 0, BUTTONS_COUNT * sizeof (unsigned char));

  unsigned int i;
  for (i = 0; i < BUTTONS_COUNT; i++)
    start_point[i] = QPoint (-1, -1);

  memset (start_time, 0, BUTTONS_COUNT * sizeof (unsigned int));
}

// Handler for \a QWidget::mouseMoveEvent().
// \sa mouse_press_event(), mouse_release_event(), mouse_double_click_event()
void mouse_filter::mouse_move_event (QMouseEvent *qevent)
{
  if (is_started_lr_click && use_is_started_lr_click)
    return;

  unsigned int i, time = get_time ();

  for (i = 0; i < BUTTONS_COUNT; i++)
    {
      if (pressed[i] && !dragged[i])
        {
          if (   time - start_time[i] >= time_to_move
              || abs (qevent->x () - start_point[i].x ()) >= move_threshold
              || abs (qevent->y () - start_point[i].y ()) >= move_threshold)
            {
              dragged[i] = 1;
              emit mouse_pressed ((mouse_button) i, start_point[i], qevent->modifiers ());
            }
        }
    }

  emit mouse_moved (dragged, qevent->pos (), qevent->modifiers ());
}

// Handler for \a QWidget::mousePressEvent().
// \sa mouse_move_event(), mouse_release_event(), mouse_double_click_event()
void mouse_filter::mouse_press_event (QMouseEvent *qevent)
{
  unsigned int i;
  if ((i = qt_mouse_button_2_mouse_filter_button (qevent->button ())) != (unsigned int)BUTTON_UNKNOWN)
    {
      pressed[i] = 1;
      start_point[i] = qevent->pos ();
      start_time[i]  = get_time ();
    }

  if (use_is_started_lr_click && pressed[mouse_filter::BUTTON_RIGHT] && pressed[mouse_filter::BUTTON_LEFT])
    is_started_lr_click = 1;
}

// Handler for \a QWidget::mouseReleaseEvent().
// \sa mouse_press_event(), mouse_move_event(), mouse_double_click_event()
void mouse_filter::mouse_release_event (QMouseEvent *qevent)
{
  unsigned int i;
  if ((i = qt_mouse_button_2_mouse_filter_button (qevent->button ())) != (unsigned int)BUTTON_UNKNOWN)
    {
      if (is_started_lr_click && use_is_started_lr_click)
        {
          dragged[i] = 0;
          pressed[i] = 0;
          if (!pressed[mouse_filter::BUTTON_RIGHT] && !pressed[mouse_filter::BUTTON_LEFT])
            {
              is_started_lr_click = 0;
              emit mouse_lr_clicked ();
            }
        }
      else if (dragged[i])
        {
          emit mouse_released ((mouse_button)i, qevent->pos (), qevent->modifiers ());
          dragged[i] = 0;
          pressed[i] = 0;
        }
      else if (pressed[i])
        {
          emit mouse_clicked ((mouse_button)i, start_point[i], qevent->modifiers ());
          pressed[i] = 0;
        }
    }
}

// Handler for \a QWidget::mouseDoubleClickEvent().
// \sa mouse_press_event(), mouse_move_event(), mouse_release_event()
void mouse_filter::mouse_double_click_event (QMouseEvent *qevent)
{
  unsigned int i;
  if ((i = qt_mouse_button_2_mouse_filter_button (qevent->button ())) != (unsigned int)BUTTON_UNKNOWN)
    emit mouse_double_clicked ((mouse_button)i, qevent->pos (), qevent->modifiers ());
}



