#ifndef EVENT_CONTAINER_CHANGED_H
#define EVENT_CONTAINER_CHANGED_H

#include "renderer/abstract_renderer_event.h"
#include "renderer/renderer_thread.h"

class renderer_items_container;

class event_container_changed : public abstract_renderer_event
{
  renderer_items_container *m_container;
public:
  event_container_changed (renderer_items_container *container)
  {
    m_container = container;
  }

  ~event_container_changed () {}

  virtual void process (renderer_thread *renderer) override
  {
    return renderer->container_changed (m_container);
  }

  virtual bool interrupt_rendering () const override { return true; }
};

#endif // EVENT_CONTAINER_CHANGED_H
