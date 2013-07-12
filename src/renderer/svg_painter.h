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
class events_queue;
class overlay_renderer;


class svg_painter : public abstract_painter
{
  QTransform m_cur_transform;
  QTransform m_last_transform;
  QPoint m_drag_start_pos;
  svg_document *m_document;
  bool drag_started;
  rendered_items_cache *m_cache;
  events_queue *m_queue;
  overlay_renderer *m_overlay;

public:
  svg_painter (gl_widget *glwidget, const mouse_filter *mouse_filter_object, rendered_items_cache *cache, events_queue *queue);
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
private:
  void reset_transform ();
  void send_changes (bool interrrupt_rendering);
  void get_cache_id (const QTransform &transform, render_cache_id &first, render_cache_id &last, const QRectF &rect) const;
  abstract_svg_item *get_current_item (const QPoint &pos);
  void update_drawing (QTransform transform);
  void draw_base (QPainter &painter);
  void draw_overlay (QPainter &painter);
};

#endif // SVG_PAINTER_H
