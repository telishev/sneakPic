#include "gui/mouse_filter.h"

#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else // !_WIN32
#include <sys/time.h>
#endif // !_WIN32

#include <QMouseEvent>
#include "mouse_shortcut.h"

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

mouse_filter::mouse_filter (QObject *parent, int move_threshold, unsigned int time_to_move)
  : QObject (parent),
    move_threshold (move_threshold),
    time_to_move (time_to_move)
{
  memset (pressed, 0, (int)mouse_button::BUTTONS_COUNT * sizeof (unsigned char));
  memset (dragged, 0, (int)mouse_button::BUTTONS_COUNT * sizeof (unsigned char));

  unsigned int i;
  for (i = 0; i < (int)mouse_button::BUTTONS_COUNT; i++)
    start_point[i] = QPoint (-1, -1);

  memset (start_time, 0, (int)mouse_button::BUTTONS_COUNT * sizeof (unsigned int));
}

mouse_filter::~mouse_filter ()
{
}

void mouse_filter::mouse_move_event (QMouseEvent *qevent)
{

  unsigned int i, time = get_time ();

  for (i = 0; i < (int)mouse_button::BUTTONS_COUNT; i++)
    {
      if (pressed[i] && !dragged[i])
        {
          if (   time - start_time[i] >= time_to_move
                 || abs (qevent->x () - start_point[i].x ()) >= move_threshold
                 || abs (qevent->y () - start_point[i].y ()) >= move_threshold)
            {
              dragged[i] = 1;
              emit_mouse_event (start_point[i], qevent->modifiers (), mouse_event_type::DRAG_START, (mouse_button) i);
            }
        }
    }

  mouse_button last_dragged = last_dragged_button ();
  if (last_dragged != mouse_button::NO_BUTTON)
    emit_mouse_event (qevent->pos (), qevent->modifiers (), mouse_event_type::DRAG, last_dragged);
  else
    emit_mouse_event (qevent->pos (), qevent->modifiers (), mouse_event_type::MOVE, mouse_button::NO_BUTTON);
}

// Handler for \a QWidget::mousePressEvent().
// \sa mouse_move_event(), mouse_release_event(), mouse_double_click_event()
void mouse_filter::mouse_press_event (QMouseEvent *qevent)
{
  mouse_button button = to_mouse_button (qevent->button ());
  if (button == mouse_button::NO_BUTTON)
    return;

  int i = (int)button;
  pressed[i] = 1;
  start_point[i] = qevent->pos ();
  start_time[i]  = get_time ();
}

// Handler for \a QWidget::mouseReleaseEvent().
// \sa mouse_press_event(), mouse_move_event(), mouse_double_click_event()
void mouse_filter::mouse_release_event (QMouseEvent *qevent)
{
  mouse_button button = to_mouse_button (qevent->button ());
  if (button == mouse_button::NO_BUTTON)
    return;

  int i = (int)button;
  if (dragged[i])
    {
      emit_mouse_event (qevent->pos (), qevent->modifiers (), mouse_event_type::DRAG_END, button);
      dragged[i] = 0;
      pressed[i] = 0;
    }
  else if (pressed[i])
    {
      emit_mouse_event (start_point[i], qevent->modifiers (), mouse_event_type::CLICK, button);
      pressed[i] = 0;
    }
}

// Handler for \a QWidget::mouseDoubleClickEvent().
// \sa mouse_press_event(), mouse_move_event(), mouse_release_event()
void mouse_filter::mouse_double_click_event (QMouseEvent *qevent)
{
  mouse_button button = to_mouse_button (qevent->button ());
  if (button == mouse_button::NO_BUTTON)
    return;

  emit_mouse_event (qevent->pos (), qevent->modifiers (), mouse_event_type::DOUBLECLICK, button);
}

mouse_button mouse_filter::to_mouse_button (Qt::MouseButton qmousebutton) const
{
  switch (qmousebutton)
    {
    case Qt::LeftButton:
      return mouse_button::BUTTON_LEFT;
    case Qt::RightButton:
      return mouse_button::BUTTON_RIGHT;
    case Qt::MidButton:
      return mouse_button::BUTTON_MIDDLE;
    default:
      return mouse_button::NO_BUTTON;
    }

  return mouse_button::NO_BUTTON;
}

bool mouse_filter::process_event (QEvent *qevent)
{
  switch (qevent->type ())
    {
    case QEvent::MouseButtonPress:
      mouse_press_event (static_cast<QMouseEvent *> (qevent));
      return true;
    case QEvent::MouseButtonRelease:
      mouse_release_event (static_cast<QMouseEvent *> (qevent));
      return true;
    case QEvent::MouseButtonDblClick:
      mouse_double_click_event (static_cast<QMouseEvent *> (qevent));
      return true;
    case QEvent::MouseMove:
      mouse_move_event (static_cast<QMouseEvent *> (qevent));
      return true;
    default:
      return false;
    }
}

keyboard_modifier mouse_filter::to_keyboard_modifier (Qt::KeyboardModifiers modifiers) const
{
  bool has_shift = modifiers.testFlag (Qt::ShiftModifier);
  bool has_alt = modifiers.testFlag (Qt::AltModifier);
  bool has_control = modifiers.testFlag (Qt::ControlModifier);
  if (has_shift && has_alt && has_control)
    return keyboard_modifier::CTRL_ALT_SHIFT;
  if (has_shift && has_alt)
    return keyboard_modifier::SHIFT_ALT;
  if (has_alt && has_control)
    return keyboard_modifier::CTRL_ALT;
  if (has_shift && has_control)
    return keyboard_modifier::CTRL_SHIFT;
  if (has_shift)
    return keyboard_modifier::SHIFT;
  if (has_alt)
    return keyboard_modifier::ALT;
  if (has_control)
    return keyboard_modifier::CTRL;

  return keyboard_modifier::NO_MODIFIERS;
}

void mouse_filter::emit_mouse_event (const QPoint &pos, Qt::KeyboardModifiers modifiers, mouse_event_type event_type, mouse_button button)
{
  mouse_event_t ev (pos, to_keyboard_modifier (modifiers), event_type, button);
  emit mouse_event_happened (ev);
}

mouse_button mouse_filter::last_dragged_button () const
{
  mouse_button last_button = mouse_button::NO_BUTTON;
  unsigned int last_time = 0;
  for (int i = 0; i < (int)mouse_button::BUTTONS_COUNT; i++)
    {
      if (!dragged[i])
        continue;

      if (start_time[i] > last_time)
        {
          last_time = start_time[i];
          last_button = (mouse_button)i;
        }
    }

  return last_button;
}

