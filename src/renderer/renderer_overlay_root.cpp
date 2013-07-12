#include "renderer/renderer_overlay_root.h"


#include <QRectF>

#include "renderer/renderer_items_container.h"
#include "renderer/renderer_config.h"


renderer_overlay_root::renderer_overlay_root (const std::string &name)
  : abstract_renderer_item (name)
{

}

renderer_overlay_root::~renderer_overlay_root ()
{

}

void renderer_overlay_root::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  for (const std::string &child_name : m_children)
    {
      abstract_renderer_item *child_item = container ()->item (child_name);
      if (child_item)
        child_item->draw (canvas, state, config);
    }
}

QRectF renderer_overlay_root::bounding_box () const 
{
  return m_bbox;
}

void renderer_overlay_root::update_bbox ()
{
  QRectF new_box;
  for (const std::string &child_name : m_children)
    {
      abstract_renderer_item *renderer_item = container ()->item (child_name);
      if (!renderer_item)
        continue;

      renderer_item->update_bbox ();
      new_box = new_box.united (renderer_item->bounding_box ());
    }

  m_bbox = new_box;
}
