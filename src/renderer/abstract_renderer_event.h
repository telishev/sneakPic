#ifndef ABSTRACT_RENDERER_EVENT_H
#define ABSTRACT_RENDERER_EVENT_H

class renderer_thread;

class abstract_renderer_event
{
  int m_queue_id;
public:
  virtual ~abstract_renderer_event () {}

  void set_queue_id (int queue_id) { m_queue_id = queue_id; }
  int queue_id () const { return m_queue_id; }

  virtual void process (renderer_thread *renderer) = 0;
  virtual bool interrupt_rendering () const = 0;
};

#endif // ABSTRACT_RENDERER_EVENT_H
