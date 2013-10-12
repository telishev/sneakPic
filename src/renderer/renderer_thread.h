#ifndef RENDERER_THREAD_H
#define RENDERER_THREAD_H

#include <QThread>
#include "renderer/render_cache_id.h"

class svg_renderer;
class renderer_items_container;
class render_cache_id;
class events_queue;

#include <QTransform>
 
class renderer_thread : public QThread
{
  static const int m_wait_time = 20;

  svg_renderer *m_renderer;
  renderer_items_container *m_container;
  bool m_exit_needed;
  events_queue *m_queue;

  render_cache_id m_first_id_to_change, m_last_id_to_change;
  QTransform m_last_transform;

public:
  renderer_thread (svg_renderer *renderer, events_queue *queue, QObject *parent = 0);
  ~renderer_thread ();

  void set_exit_needed () { m_exit_needed = true; }

  void transform_changed (const render_cache_id &first, const render_cache_id &last, const QTransform &transform);
  void container_changed (renderer_items_container *container);

  renderer_items_container *container () const { return m_container; }

  void invalidate_rect (const QRectF &rect);

protected:
  virtual void run () override;

private:
  void redraw ();
  void small_wait ();
};

#endif // RENDERER_THREAD_H
