#include "renderer/renderer_config.h"

#include "renderer/events_queue.h"


renderer_config::renderer_config (events_queue *queue)
{
  m_queue = queue;
}

renderer_config::~renderer_config ()
{

}

bool renderer_config::is_interrupted () const
{
  return m_queue->is_interrupted ();
}
