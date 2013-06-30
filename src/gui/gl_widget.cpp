#include "gui/gl_widget.h"

#include <QPainter>
#include <QPaintEvent>

#include "common/memory_deallocation.h"

#include "gui/mouse_filter.h"

#include "renderer/abstract_painter.h"



gl_widget::gl_widget (QWidget *parent)
  //: QGLWidget (QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel), parent)
  : QWidget (parent)
{
  m_cur_painter = nullptr;
  setAutoFillBackground (false);

  m_mouse_filter_object = new mouse_filter (this);
  
  connect (m_mouse_filter_object, SIGNAL (mouse_moved (const unsigned char *, const QPoint &, const Qt::KeyboardModifiers &)),
            this, SLOT (mouse_moved (const unsigned char *, const QPoint &, const Qt::KeyboardModifiers &)));
  connect (m_mouse_filter_object, SIGNAL (mouse_clicked (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)),
            this, SLOT (mouse_clicked (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)));
  connect (m_mouse_filter_object, SIGNAL (mouse_double_clicked (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)),
            this, SLOT (mouse_double_clicked (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)));
  connect (m_mouse_filter_object, SIGNAL (mouse_pressed (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)),
            this, SLOT (mouse_pressed (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)));
  connect (m_mouse_filter_object, SIGNAL (mouse_released (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)),
            this, SLOT (mouse_released (mouse_filter::mouse_button, const QPoint &, const Qt::KeyboardModifiers &)));

   setMouseTracking (true);
}

gl_widget::~gl_widget ()
{
  FREE (m_cur_painter);
}

void gl_widget::paintEvent (QPaintEvent * /*qevent*/)
{
  if (!m_cur_painter)
    return;

  m_cur_painter->configure ();
  m_cur_painter->draw ();
}

void gl_widget::mousePressEvent (QMouseEvent *qevent)
{
  m_mouse_filter_object->mouse_press_event (qevent);
  qevent->accept ();
}

void gl_widget::mouseMoveEvent (QMouseEvent *qevent)
{
  m_mouse_filter_object->mouse_move_event (qevent);

  qevent->accept ();
}

void gl_widget::mouseReleaseEvent (QMouseEvent *qevent)
{
  m_mouse_filter_object->mouse_release_event (qevent);

  qevent->accept ();
}

void gl_widget::mouseDoubleClickEvent (QMouseEvent *qevent)
{
  m_mouse_filter_object->mouse_double_click_event (qevent);

  qevent->accept ();
}

void gl_widget::mouse_moved (const unsigned char *dragging_buttons, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  if (m_cur_painter)
    m_cur_painter->mouse_moved (dragging_buttons, pos, modifiers);
}

void gl_widget::mouse_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  if (m_cur_painter)
    m_cur_painter->mouse_clicked (button, pos, modifiers);
}

void gl_widget::mouse_double_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  if (m_cur_painter)
    m_cur_painter->mouse_double_clicked (button, pos, modifiers);
}

void gl_widget::mouse_pressed (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  if (m_cur_painter)
    m_cur_painter->mouse_pressed (button, pos, modifiers);
}

void gl_widget::mouse_released (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  if (m_cur_painter)
    m_cur_painter->mouse_released (button, pos, modifiers);
}

/// analyse mouse wheel scrolling in GL area
void gl_widget::wheelEvent (QWheelEvent *qevent)
{
  if (m_cur_painter)
    m_cur_painter->wheelEvent (qevent);
  qevent->accept ();
}

/// If mouse leaves widget area
void gl_widget::leaveEvent (QEvent *qevent)
{
  if (m_cur_painter)
    m_cur_painter->leaveEvent (qevent);
  qevent->accept ();
}

