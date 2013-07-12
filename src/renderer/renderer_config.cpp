#include "renderer/renderer_config.h"

#include "renderer/events_queue.h"


renderer_config::renderer_config ()
{
  m_queue = nullptr;
  m_use_new_cache = false;
  m_render_for_selection = false;
}

renderer_config::~renderer_config ()
{

}

bool renderer_config::is_interrupted () const
{
  return m_queue ? m_queue->is_interrupted () : false;
}
