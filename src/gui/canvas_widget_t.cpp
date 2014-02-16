#include "gui/canvas_widget_t.h"

#include <QPainter>
#include <QPaintEvent>

#include "common/memory_deallocation.h"

#include "gui/mouse_filter.h"
#include "gui/connection.h"

#include "renderer/abstract_painter.h"



canvas_widget_t::canvas_widget_t (QWidget *parent)
  //: QGLWidget (QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel), parent)
  : QWidget (parent)
{
  m_cur_painter = nullptr;
  setAutoFillBackground (false);
  setFocusPolicy (Qt::ClickFocus);

  m_mouse_filter_object = new mouse_filter (this);

  CONNECT (m_mouse_filter_object, &mouse_filter::mouse_event_happened, this, &canvas_widget_t::mouse_event);

  setMouseTracking (true);
}

canvas_widget_t::~canvas_widget_t ()
{
}

void canvas_widget_t::paintEvent (QPaintEvent * /*qevent*/)
{
  if (!m_cur_painter)
    {
      QPainter painter (this);
      painter.fillRect (rect (), Qt::white);
      return;
    }

  m_cur_painter->configure ();
  m_cur_painter->set_configure_all (false);
  m_cur_painter->draw ();
}

void canvas_widget_t::wheelEvent (QWheelEvent *qevent)
{
  if (m_cur_painter)
    m_cur_painter->wheelEvent (qevent);
  qevent->accept ();
}

const QPoint &canvas_widget_t::cursor_pos ()
{
  return mapFromGlobal (QCursor::pos ());
};

/// If mouse leaves widget area
void canvas_widget_t::leaveEvent (QEvent *qevent)
{
  if (m_cur_painter)
    m_cur_painter->leaveEvent (qevent);
  qevent->accept ();
}

void canvas_widget_t::resizeEvent (QResizeEvent *qevent)
{
  if (m_cur_painter)
    m_cur_painter->resizeEvent (qevent);

  QWidget::resizeEvent (qevent);
}

bool canvas_widget_t::event (QEvent *qevent)
{
  if (m_mouse_filter_object->process_event (qevent))
    {
      qevent->accept ();
      return true;
    }

  return QWidget::event (qevent);
}

void canvas_widget_t::mouse_event (const mouse_event_t &m_event)
{
  if (!m_cur_painter)
    return;

  m_cur_painter->mouse_event (m_event);
}
