#include "editor/path_handles.h"

#include "renderer/overlay_renderer.h"
#include "renderer/overlay_item_type.h"

#include "editor/items_selection.h"

path_handles::path_handles (const items_selection *selection, overlay_renderer *overlay, svg_items_container *container)
  : overlay_items_container (overlay, container, overlay_layer_type::TEMP)
{
  m_selection = selection;
}

path_handles::~path_handles ()
{

}

renderable_item *path_handles::create_overlay_item (const std::string &object) const 
{
  return create_overlay_for_item (object, overlay_item_type::HANDLES);
}

void path_handles::update ()
{
  clear_items ();
  for (const auto &item : m_selection->selection ())
    add_svg_item (item);
}


