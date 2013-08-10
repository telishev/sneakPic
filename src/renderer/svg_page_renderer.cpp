#include "renderer/svg_page_renderer.h"

#include "renderer/overlay_renderer.h"
#include "renderer/renderer_page.h"

#include "svg/svg_document.h"


svg_page_renderer::svg_page_renderer (overlay_renderer *overlay)
  : overlay_items_container (overlay, overlay_layer_type::PAGE)
{
  svg_item_changed ("");
}

svg_page_renderer::~svg_page_renderer ()
{

}

std::vector<abstract_renderer_item *> svg_page_renderer::create_overlay_item (const std::string & /*object*/) const 
{
  std::vector<abstract_renderer_item *> result;
  if (!document ())
    return result;
  
  renderer_page *page_item = new renderer_page ("#page_item");
  double width, height;
  document ()->get_doc_dimensions (width, height);
  page_item->set_height (height);
  page_item->set_width (width);
  result.push_back (page_item);
  return result;
}

