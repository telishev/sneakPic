#include "renderer/current_item_outline_renderer.h"

#include "renderer/overlay_renderer.h"
#include "renderer/overlay_item_type.h"


current_item_outline_renderer::current_item_outline_renderer (svg_items_container *container)
  : overlay_items_container (container)
{

}

current_item_outline_renderer::~current_item_outline_renderer ()
{

}

void current_item_outline_renderer::set_current_item (const string &item)
{
  remove_svg_item (m_item);
  m_item = item;
  add_svg_item (m_item);
}

renderable_item *current_item_outline_renderer::create_overlay_item (const string &object) const 
{
  return create_overlay_for_item (object, overlay_item_type::CURRENT_ITEM);
}
