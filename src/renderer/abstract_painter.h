/// \file renderer/abstract_painter.h
/// \brief class used to draw something on glwidget

#ifndef ABSTRACT_PAINTER_H
#define ABSTRACT_PAINTER_H

#include "gui/mouse_filter.h"

#include <QObject>

class gl_widget;
class QWheelEvent;
class QEvent;
class QKeyEvent;
class QResizeEvent;

class mouse_event_t;

enum class configure_type
{
  REDRAW_BASE,
  ITEMS_CHANGED,
  SELECTION_CHANGED,

  COUNT,
};

class abstract_painter : public QObject
{
  Q_OBJECT

  gl_widget          *m_glwidget;
  int m_config_needed[(int)configure_type::COUNT];

public:
  abstract_painter (gl_widget *glwidget);
  virtual ~abstract_painter ();

  /// mouse events
  virtual unsigned int mouse_event (const mouse_event_t &m_event) = 0;

  virtual void draw () = 0;
  /// must check for config_needed values and configure only parts that needs reconfigure
  virtual void configure () = 0;

  /// \note event functions should return false if event was not processed; If you do not kwno how to process event, do nothing and return false
  virtual void wheelEvent (QWheelEvent *qevent) = 0;
  virtual void leaveEvent (QEvent *qevent) = 0;
  virtual bool event (QEvent *qevent) = 0;

  virtual void resizeEvent (QResizeEvent *qevent) = 0;

  gl_widget *glwidget () const { return m_glwidget; }

  void update ();

  void set_configure_needed (configure_type type, int value = 1) { m_config_needed[(int)type] = value; }
  int get_configure_needed (configure_type type) const { return m_config_needed[(int)type]; }
  void set_configure_all (int value);
};

#endif // ABSTRACT_PAINTER_H
