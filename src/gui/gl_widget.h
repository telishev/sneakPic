#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QGLWidget>
#include <QGLFunctions>
#include "gui/mouse_filter.h"

class mouse_filter;
class abstract_painter;

class gl_widget : public QGLWidget, protected QGLFunctions
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
  void mouse_moved (const unsigned char *dragging_buttons, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);
  void mouse_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);
  void mouse_double_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);
  void mouse_pressed (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);
  void mouse_released (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers);

protected:
  virtual void paintEvent (QPaintEvent *qevent) override;
  virtual void mousePressEvent (QMouseEvent *qevent) override;
  virtual void mouseMoveEvent (QMouseEvent *qevent) override;
  virtual void mouseReleaseEvent (QMouseEvent *qevent) override;
  virtual void mouseDoubleClickEvent (QMouseEvent *qevent) override;
  virtual void wheelEvent (QWheelEvent *qevent) override;
  virtual void leaveEvent (QEvent *qevent) override;
};



#endif // GL_WIDGET_H
