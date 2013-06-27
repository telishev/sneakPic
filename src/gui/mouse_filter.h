/// \file  gui/mouse_filter.h
/// \brief This class eliminates small cursor positioning mistakes, e.g. 1-2 pixels accidental move when user presses mouse button.
///        All mouse events should be provided in this class by class methods calling.  Any mouse state should be took from signals
///        of this class.

#ifndef MOUSE_FILTER_H
#define MOUSE_FILTER_H

class mouse_filter;

#include <QObject>

#include <QMouseEvent>
#include <QPoint>


/*************************************************************************************************\
* brief: class mouse_filter                                                                       *
* desc:  This class eliminates small cursor positioning mistakes.                                 *
\*************************************************************************************************/
class mouse_filter : public QObject
{
  Q_OBJECT

public:
  enum mouse_button
  {
    BUTTON_UNKNOWN = -1,

    BUTTON_LEFT = 0,
    BUTTON_RIGHT,
    BUTTON_MIDDLE,

    BUTTONS_COUNT,
  };

  // Protected Membres
protected:
  int move_threshold;                 ///< Threshold for accidental move when user presses mouse button (in pixels).
  unsigned int time_to_move;          ///< Accidental moves eliminating are enabled during this time (in milliseconds) after mouse button pressed.

  unsigned char pressed[BUTTONS_COUNT];   ///< Current mouse buttons pressed states array.
  unsigned char dragged[BUTTONS_COUNT];   ///< Current mouse buttons dragged states array. 1 - mouse is being moved and button is being pressed.

  QPoint       start_point[BUTTONS_COUNT];///< Start points array. Start point - point where button was pressed (in pixels).
  unsigned int start_time[BUTTONS_COUNT]; ///< Start time array. Start time - time when button was pressed (in milliseconds).
  int is_started_lr_click;
  int use_is_started_lr_click;

  // //////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// Initializing Constructor.
  ///
  mouse_filter (QObject *parent, int _use_is_started_lr_click = 0, int move_threshold = 2, unsigned int time_to_move = 400);
  /// Destructor.
  ///
  ~mouse_filter ();

  // //////////////////////////////////////////////////////////////////////////////////////////////
  // Public Methods
public:
  /// Get specified mouse button pressed state.
  ///
  int/*bool*/ is_pressed (mouse_button button) const { return (int)pressed[button]; }

  /// Handler for \a QWidget::mouseMoveEvent().
  ///
  /// \sa mouse_press_event() mouse_release_event() mouse_double_click_event()
  ///
  void mouse_move_event (QMouseEvent *qevent);

  /// Handler for \a QWidget::mousePressEvent().
  ///
  /// \sa mouse_move_event() mouse_release_event() mouse_double_click_event()
  ///
  void mouse_press_event (QMouseEvent *qevent);

  /// Handler for \a QWidget::mouseReleaseEvent().
  ///
  /// \sa mouse_press_event() mouse_move_event() mouse_double_click_event()
  ///
  void mouse_release_event (QMouseEvent *qevent);

  /// Handler for \a QWidget::mouseDoubleClickEvent().
  ///
  /// \sa mouse_press_event() mouse_move_event() mouse_release_event()
  ///
  void mouse_double_click_event (QMouseEvent *qevent);

  // //////////////////////////////////////////////////////////////////////////////////////////////
  // Signals
signals:
  /// Cursor was moved.
  ///
  /// \param dragging_buttons [in] mouse buttons those are pressed while mouse is being moved.
  ///
  /// \sa get_pressed_buttons()
  ///
  void mouse_moved (const unsigned char *dragging_buttons, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);

  /// Mouse button was clicked.
  ///
  void mouse_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);

  /// Mouse button was double clicked.
  ///
  void mouse_double_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);

  /// Mouse button was pressed.
  ///
  /// \warninig \a mouse_pressed() and \a mouse_released() signals would be emited only if cursor was moved when mouse button was pressed.
  ///
  /// \sa mouse_clicked() mouse_released()
  ///
  void mouse_pressed (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);

  /// Mouse button was released.
  ///
  /// \warninig \a mouse_pressed() and \a mouse_released() signals would be emited only if cursor was moved when mouse button was pressed.
  ///
  /// \sa mouse_clicked() mouse_pressed()
  ///
  void mouse_released (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);

  void mouse_lr_clicked ();

  // //////////////////////////////////////////////////////////////////////////////////////////////
  // Protected Methods
protected:
};



#endif // MOUSE_FILTER_H
