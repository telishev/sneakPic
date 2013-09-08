#include "svg_item_group_type.h"

#include "renderer/renderer_item_group.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attribute_clip_path.h"
#include "svg/attributes/svg_attributes_enum.h"

#include "svg/items/svg_item_clip_path.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/svg_document.h"


svg_item_group_type::svg_item_group_type (svg_document *document)
  : svg_graphics_item (document)
{

}

void svg_item_group_type::update_group_item (renderer_item_group *renderer_item) const
{
  const svg_attribute_opacity *opacity = get_computed_attribute<svg_attribute_opacity> ();
  const svg_attribute_display *display = get_computed_attribute<svg_attribute_display> ();
  const svg_item_clip_path *clip_path = get_computed_attribute<svg_attribute_clip_path> ()->clip_path (document ()->item_container ());
 
  renderer_item->set_opacity (opacity->computed_opacity ());
  renderer_item->set_display (display->value ());
  renderer_item->set_transform (full_transform ());
  if (clip_path)
    renderer_item->set_clip_path (clip_path->get_clip_path ());

  for (int i = 0; i < children_count (); i++)
    renderer_item->push_back_child (child (i)->name ());
}

abstract_renderer_item *svg_item_group_type::create_overlay_item (overlay_item_type /*overlay_type*/) const
{
  return nullptr;
}

void svg_item_group_type::update_bbox_impl ()
{
  QRectF bbox;
  for (int i = 0; i < children_count (); i++)
    {
      svg_graphics_item *graphics_item = child (i)->to_graphics_item ();
      if (!graphics_item)
        continue;

      graphics_item->update_bbox ();
      bbox = bbox.united (graphics_item->bbox ());
    }

  m_bbox = bbox;
}
