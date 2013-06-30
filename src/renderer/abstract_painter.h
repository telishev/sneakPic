/// \file renderer/abstract_painter.h
/// \brief class used to draw something on glwidget

#ifndef ABSTRACT_PAINTER_H
#define ABSTRACT_PAINTER_H

#include "gui/mouse_filter.h"

class gl_widget;
class QWheelEvent;
class QEvent;

enum configure_type
{
  CONFIGURE_TYPE__REDRAW,
  CONFIGURE_TYPE__ITEMS_CHANGED,

  CONFIGURE_TYPE__COUNT,
};

class abstract_painter
{
  gl_widget          *m_glwidget;
  const mouse_filter *m_mouse_filter_object;
  int m_config_needed[CONFIGURE_TYPE__COUNT];

public:
  abstract_painter (gl_widget *glwidget, const mouse_filter *mouse_filter_object);
  virtual ~abstract_painter ();

  /// mouse events
  virtual unsigned int mouse_moved (const unsigned char *dragging_buttons, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) = 0;
  virtual unsigned int mouse_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) = 0;
  virtual unsigned int mouse_double_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) = 0;
  virtual unsigned int mouse_pressed (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) = 0;
  virtual unsigned int mouse_released (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) = 0;

  virtual void draw () = 0;
  /// must check for config_needed values and configure only parts that needs reconfigure
  virtual void configure () = 0;

  /// \note event functions should return false if event was not processed; If you do not kwno how to process event, do nothing and return false
  virtual void wheelEvent (QWheelEvent *qevent) = 0;
  virtual void leaveEvent (QEvent *qevent) = 0;
  virtual bool keyReleaseEvent (QKeyEvent *qevent) = 0;
  virtual bool event (QEvent *qevent) = 0;

  virtual void resizeGL (int width, int height) = 0;

  gl_widget *glwidget () const { return m_glwidget; }

  void set_configure_needed (configure_type type, int value) { m_config_needed[type] = value; }
  int get_configure_needed (configure_type type) const { return m_config_needed[type]; }
  void set_configure_all (int value);
};

#endif // ABSTRACT_PAINTER_H
