#include "editor/handles_renderer.h"

#include "renderer/overlay_renderer.h"
#include "renderer/overlay_item_type.h"
#include "renderer/renderable_item.h"

#include "editor/items_selection.h"
#include "editor/element_handles.h"
#include "editor/abstract_handle.h"
#include "handles_editor.h"


class handles_renderable_item : public renderable_item
{
  element_handles *m_handles;
public:
  handles_renderable_item (element_handles *handles)
    : m_handles (handles) {}

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override
  {
    for (auto handle : m_handles->handles ())
      handle->draw (canvas, state, config);
  }
};

handles_renderer::handles_renderer (const handles_editor *editor, overlay_renderer *overlay, svg_items_container *container)
  : overlay_items_container (overlay, container, overlay_layer_type::TEMP)
{
  m_editor = editor;
}

handles_renderer::~handles_renderer ()
{

}

renderable_item *handles_renderer::create_overlay_item (const std::string &object) const 
{
  element_handles *handles = m_editor->handles_for_item (object);
  if (!handles)
    return nullptr;

  return new handles_renderable_item (handles);
}


