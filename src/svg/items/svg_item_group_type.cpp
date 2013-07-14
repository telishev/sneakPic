#include "svg_item_group_type.h"

#include "renderer/renderer_item_group.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attribute_clip_path.h"
#include "svg/attributes/svg_attributes_enum.h"

#include "svg/items/svg_item_clip_path.h"


svg_item_group_type::svg_item_group_type (svg_document *document)
  : abstract_svg_item (document)
{

}

void svg_item_group_type::update_group_item (renderer_item_group *renderer_item) const
{
  const svg_attribute_opacity *opacity = get_computed_attribute<svg_attribute_opacity> ();
  const svg_attribute_transform *transform = get_computed_attribute<svg_attribute_transform> ();
  const svg_attribute_display *display = get_computed_attribute<svg_attribute_display> ();
  const svg_item_clip_path *clip_path = get_computed_attribute<svg_attribute_clip_path> ()->clip_path ();
 
  renderer_item->set_opacity (opacity->computed_opacity ());
  renderer_item->set_display (display->value ());
  renderer_item->set_transform (transform->computed_transform ());
  if (clip_path)
    renderer_item->set_clip_path (clip_path->get_clip_path ());
}
