#ifndef EVENT_TRANSFORM_CHANGED_H
#define EVENT_TRANSFORM_CHANGED_H

#include "renderer/abstract_renderer_event.h"
#include "renderer/render_cache_id.h"
#include "renderer/renderer_thread.h"

#include <QTransform>


class event_transform_changed : public abstract_renderer_event
{
  QTransform m_transform;
  render_cache_id m_first, m_last;
public:
  event_transform_changed (const render_cache_id &first, const render_cache_id &last, const QTransform &transform)
  {
    m_transform = transform;
    m_first = first;
    m_last = last;
  }

  ~event_transform_changed () {}

  virtual void process (renderer_thread *renderer) override
  {
    return renderer->transform_changed (m_first, m_last, m_transform);
  }
};

#endif // EVENT_TRANSFORM_CHANGED_H
