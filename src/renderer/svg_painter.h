#ifndef SVG_PAINTER_H
#define SVG_PAINTER_H

#include "renderer/abstract_painter.h"

#include <QTransform>

class svg_document;
class abstract_svg_item;
class abstract_renderer_item;
class rendered_items_cache;
class render_cache_id;
class svg_renderer;
class abstract_renderer_event;

template<typename T>
class wait_queue;


class svg_painter : public abstract_painter
{
  QTransform m_cur_transform;
  QTransform m_last_transform;
  QPoint m_drag_start_pos;
  svg_document *m_document;
  bool drag_started;
  rendered_items_cache *m_cache;
  wait_queue<abstract_renderer_event> *m_queue;

public:
  svg_painter (gl_widget *glwidget, const mouse_filter *mouse_filter_object, rendered_items_cache *cache, wait_queue<abstract_renderer_event> *queue);
  ~svg_painter ();

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

  void draw_items (QPainter &painter, const QRectF &rect_to_draw, QTransform transform);
private:
  void reset_transform ();
  void send_changes ();
  void get_cache_id (const QTransform &transform, render_cache_id &first, render_cache_id &last, const QRectF &rect) const;
};

#endif // SVG_PAINTER_H