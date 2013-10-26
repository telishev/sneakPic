#include "renderer/items_selection_renderer.h"

#include "renderer/overlay_renderer.h"
#include "renderer/overlay_item_type.h"
#include "editor/items_selection.h"

items_selection_renderer::items_selection_renderer (overlay_renderer *overlay, svg_items_container *container, items_selection *selection)
  : overlay_items_container (overlay, container, overlay_layer_type::BASE)
{
  m_selection = selection;
}

items_selection_renderer::~items_selection_renderer ()
{

}

renderable_item *items_selection_renderer::create_overlay_item (const std::string &object) const 
{
  return create_overlay_for_item (object, overlay_item_type::SELECTION);
}

void items_selection_renderer::update_items ()
{
  clear_items ();
  for (auto &item : m_selection->selection ())
    svg_item_changed (item);
}
