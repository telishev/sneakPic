#ifndef ABSTRACT_RENDERER_EVENT_H
#define ABSTRACT_RENDERER_EVENT_H

class renderer_thread;

class abstract_renderer_event
{
public:
  virtual ~abstract_renderer_event () {}

  virtual void process (renderer_thread *renderer) = 0;
};

#endif // ABSTRACT_RENDERER_EVENT_H
