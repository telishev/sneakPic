#include "renderer/renderer_item_group.h"

#include "svg/items/abstract_svg_item.h"




renderer_item_group::renderer_item_group (const abstract_svg_item *svg_item)
{
  m_opacity = 1.0;
  m_bbox_calculated = false;
  m_svg_item = svg_item;
}

renderer_item_group::~renderer_item_group ()
{

}

void renderer_item_group::draw (QPainter &/*painter*/) const 
{
  return;
}

QRectF renderer_item_group::bounding_box () const 
{
  if (!m_bbox_calculated)
    update_bbox ();

  return m_bbox;
}

void renderer_item_group::update_bbox () const
{
  QRectF new_box;
  for (const abstract_svg_item *child = m_svg_item->first_child (); child; child = child->next_sibling ())
    {
      const abstract_renderer_item *renderer_item = child->get_renderer_item ();
      if (!renderer_item)
        continue;

      new_box = new_box.united (renderer_item->bounding_box ());
    }

  m_bbox = new_box;
  m_bbox_calculated = true;
}
