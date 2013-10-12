#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QGLWidget>
#include <QGLFunctions>
#include "gui/mouse_filter.h"

class mouse_filter;
class abstract_painter;

class gl_widget : public QWidget, protected QGLFunctions
{
  Q_OBJECT

  mouse_filter *m_mouse_filter_object;
  abstract_painter *m_cur_painter;
public:
  gl_widget (QWidget *parent);
  ~gl_widget ();

  void set_painter (abstract_painter *painter) { m_cur_painter = painter; }
  mouse_filter *mouse_filter_object () const { return m_mouse_filter_object; }

private slots:
  void mouse_event (const mouse_event_t &m_event);

protected:
  virtual void paintEvent (QPaintEvent *qevent) override;
  virtual void wheelEvent (QWheelEvent *qevent) override;
  virtual void leaveEvent (QEvent *qevent) override;
  virtual void resizeEvent (QResizeEvent *qevent) override;
  virtual bool event (QEvent *qevent) override;
};

#endif // GL_WIDGET_H
