#ifndef MOUSE_FILTER_H
#define MOUSE_FILTER_H

#include <QObject>
#include <QPoint>

#include "gui/mouse_shortcut.h"

class QEvent;
class QMouseEvent;

class mouse_event_t
{
  QPoint m_pos;
  keyboard_modifier m_modifier;
  mouse_event_type m_event_type;
  mouse_button m_button;
public:
  mouse_event_t (const QPoint &pos, keyboard_modifier modifier, mouse_event_type event_type, mouse_button button)
    : m_pos (pos), m_modifier (modifier), m_event_type (event_type), m_button (button) {}

  QPoint pos () const { return m_pos; }
  keyboard_modifier modifier () const { return m_modifier; }
  mouse_event_type event_type () const { return m_event_type; }
  mouse_button button () const { return m_button; }
};

class mouse_filter : public QObject
{
  Q_OBJECT

  const int move_threshold;                 ///< Threshold for accidental move when user presses mouse button (in pixels).
  const unsigned int time_to_move;          ///< Accidental moves eliminating are enabled during this time (in milliseconds) after mouse button pressed.

  unsigned char pressed[(int) mouse_button::BUTTONS_COUNT];   ///< Current mouse buttons pressed states array.
  unsigned char dragged[(int) mouse_button::BUTTONS_COUNT];   ///< Current mouse buttons dragged states array. 1 - mouse is being moved and button is being pressed.

  QPoint       start_point[(int) mouse_button::BUTTONS_COUNT];///< Start points array. Start point - point where button was pressed (in pixels).
  unsigned int start_time[(int) mouse_button::BUTTONS_COUNT]; ///< Start time array. Start time - time when button was pressed (in milliseconds).

public:
  mouse_filter (QObject *parent, int move_threshold = 2, unsigned int time_to_move = 400);
  ~mouse_filter ();

  bool process_event (QEvent *qevent);

signals:
  void mouse_event_happened (const mouse_event_t &m_event);

private:
  void mouse_move_event (QMouseEvent *qevent);
  void mouse_press_event (QMouseEvent *qevent);
  void mouse_release_event (QMouseEvent *qevent);
  void mouse_double_click_event (QMouseEvent *qevent);
  mouse_button to_mouse_button (Qt::MouseButton qmousebutton) const;
  keyboard_modifier to_keyboard_modifier (Qt::KeyboardModifiers modifiers) const;
  void emit_mouse_event (const QPoint &pos, Qt::KeyboardModifiers modifiers, mouse_event_type event_type, mouse_button button);
  mouse_button last_dragged_button () const;
};



#endif // MOUSE_FILTER_H
