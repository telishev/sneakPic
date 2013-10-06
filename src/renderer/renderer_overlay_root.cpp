#include "renderer/renderer_overlay_root.h"

#include "common/memory_deallocation.h"


renderer_overlay_root::renderer_overlay_root ()
{

}

renderer_overlay_root::~renderer_overlay_root ()
{
  for (renderable_item *item : m_items)
    {
      FREE (item);
    }
}

void renderer_overlay_root::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  for (renderable_item *item : m_items)
    {
      item->draw (canvas, state, config);
    }
}

void renderer_overlay_root::add_item (renderable_item *item)
{
  m_items.insert (item);
}

void renderer_overlay_root::remove_item (renderable_item *item)
{
  m_items.erase (item);
  FREE (item);
}

bool renderer_overlay_root::empty () const
{
  return m_items.empty ();
}

