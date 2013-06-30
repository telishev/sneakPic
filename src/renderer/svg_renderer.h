#ifndef SVG_RENDERER_H
#define SVG_RENDERER_H

#include "renderer/abstract_painter.h"

#include <QTransform>

class svg_document;

class svg_renderer : public abstract_painter
{
  QTransform m_cur_transform;
  QTransform m_last_transform;
  QPoint m_drag_start_pos;
  svg_document *m_document;
  bool drag_started;
public:
  svg_renderer (gl_widget *glwidget, const mouse_filter *mouse_filter_object);
  ~svg_renderer ();

  void set_document (svg_document *document);

  virtual unsigned int mouse_moved (const unsigned char *dragging_buttons, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) override;
  virtual unsigned int mouse_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) override;
  virtual unsigned int mouse_double_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) override;
  virtual unsigned int mouse_pressed (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) override;
  virtual unsigned int mouse_released (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers) override;
  virtual void draw () override;
  virtual void configure () override;

  virtual void wheelEvent (QWheelEvent *qevent) override;
  virtual void leaveEvent (QEvent *qevent) override;
  virtual void resizeGL (int width, int height) override;
  virtual bool event (QEvent * /*qevent*/) override { return false; }
  virtual bool keyReleaseEvent (QKeyEvent *qevent) override;

private:
  void reset_transform ();
};

#endif // SVG_RENDERER_H
