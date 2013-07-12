#ifndef RENDERER_CONFIG_H
#define RENDERER_CONFIG_H

class events_queue;

class renderer_config
{
  events_queue *m_queue;
  bool m_use_new_cache;
  bool m_render_for_selection;
public:
  renderer_config ();
  ~renderer_config ();

  void set_use_new_cache (bool use) { m_use_new_cache = use; }
  void set_render_for_selection (bool render_for_selection) { m_render_for_selection = render_for_selection; }
  void set_queue (events_queue *queue) { m_queue = queue; }
  bool is_interrupted () const;

  bool use_new_cache () const { return m_use_new_cache; }
  bool render_for_selection () const { return m_render_for_selection; }
};

#endif // RENDERER_CONFIG_H
