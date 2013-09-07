#include "renderer/svg_page_renderer.h"

#include "renderer/overlay_renderer.h"
#include "renderer/renderer_page.h"


svg_page_renderer::svg_page_renderer (overlay_renderer *overlay)
  : overlay_items_container (overlay, overlay_layer_type::PAGE)
{
  m_width = m_height = 0.0;
}

svg_page_renderer::~svg_page_renderer ()
{

}

void svg_page_renderer::set_dimensions (double width, double height)
{
  m_height = height;
  m_width = width;
  svg_item_changed ("");
}

std::vector<abstract_renderer_item *> svg_page_renderer::create_overlay_item (const std::string & /*object*/) const 
{
  std::vector<abstract_renderer_item *> result;
  if (!svg_container ())
    return result;
  
  renderer_page *page_item = new renderer_page ("#page_item");
  page_item->set_height (m_height);
  page_item->set_width (m_width);
  result.push_back (page_item);
  return result;
}

