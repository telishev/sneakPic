#include "renderer/transformed_renderable_items.h"

#include "renderer/renderer_state.h"


transformed_renderable_items::transformed_renderable_items ()
{
}

transformed_renderable_items::~transformed_renderable_items ()
{

}

void transformed_renderable_items::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  if (m_base_items.empty ())
    return;

  renderer_state new_state (state.rect (), m_transform *state.transform ());
  for (auto &item : m_base_items)
    item->draw (canvas, new_state, config);
}


void transformed_renderable_items::set_base_item_transform (const QTransform &transform)
{
  m_transform = transform;
}

void transformed_renderable_items::add_base_item (renderable_item *base_item)
{
  m_base_items.push_back (std::unique_ptr<renderable_item> (base_item));
}

void transformed_renderable_items::clear_base_items ()
{
  m_base_items.clear ();
}
