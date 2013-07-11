#ifndef RENDERER_CONFIG_H
#define RENDERER_CONFIG_H

class events_queue;

class renderer_config
{
  events_queue *m_queue;
public:
  renderer_config (events_queue *queue);
  ~renderer_config ();

  bool is_interrupted () const;
};

#endif // RENDERER_CONFIG_H
