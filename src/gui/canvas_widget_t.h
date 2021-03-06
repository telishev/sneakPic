#ifndef CANVAS_WIDGET_T_H
#define CANVAS_WIDGET_T_H

#include <QGLWidget>
#include "gui/mouse_filter.h"

class mouse_filter;
class abstract_painter;

class canvas_widget_t : public QWidget
{
  Q_OBJECT

  mouse_filter *m_mouse_filter_object;
  abstract_painter *m_cur_painter;
public:
  canvas_widget_t (QWidget *parent);
  ~canvas_widget_t ();

  void set_painter (abstract_painter *painter) { m_cur_painter = painter; }
  mouse_filter *mouse_filter_object () const { return m_mouse_filter_object; }
  QPoint cursor_pos ();

private slots:
  void mouse_event (const mouse_event_t &m_event);

protected:
  virtual void paintEvent (QPaintEvent *qevent) override;
  virtual void wheelEvent (QWheelEvent *qevent) override;
  virtual void leaveEvent (QEvent *qevent) override;
  virtual void resizeEvent (QResizeEvent *qevent) override;
  virtual bool event (QEvent *qevent) override;
};

#endif // CANVAS_WIDGET_T_H
