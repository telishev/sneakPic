#include "renderer/overlay_item_group.h"


overlay_item_group::overlay_item_group ()
{

}

overlay_item_group::~overlay_item_group ()
{

}

void overlay_item_group::add_child (renderable_item *child)
{
  m_children.push_back (std::move (unique_ptr<renderable_item> (child)));
}

void overlay_item_group::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  for (auto &child : m_children)
    child->draw (canvas, state, config);
}
