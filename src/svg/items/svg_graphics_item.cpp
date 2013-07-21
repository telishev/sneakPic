#include "svg/items/svg_graphics_item.h"

#include "svg/attributes/svg_attribute_clip_path.h"
#include "svg/attributes/svg_attribute_transform.h"

#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attributes_number.h"

#include "svg/svg_document.h"

#include "svg/items/svg_item_clip_path.h"

#include "renderer/renderer_graphics_item.h"
#include "renderer/renderer_overlay_path.h"
#include "renderer/overlay_item_type.h"

#include <memory>
#include "renderer/renderer_item_selection.h"

svg_graphics_item::svg_graphics_item (svg_document *document)
  : abstract_svg_item (document)
{
}

svg_graphics_item::~svg_graphics_item ()
{
}

abstract_renderer_item *svg_graphics_item::create_overlay_item (overlay_item_type overlay_type) const 
{
  switch (overlay_type)
    {
    case overlay_item_type::CURRENT_ITEM:
      {
        return create_outline_renderer ();
      }
    case overlay_item_type::SELECTION:
      {
        renderer_item_selection *selection = new renderer_item_selection (document ()->create_overlay_name ().toStdString ());
        selection->set_bbox (bbox_with_clip (true));
        return selection;
      }
    }
  return nullptr;
}

void svg_graphics_item::set_item_style (renderer_graphics_item *item) const 
{
  const svg_attribute_transform *transform = get_computed_attribute<svg_attribute_transform> ();
  const svg_item_clip_path *clip_path = get_computed_attribute<svg_attribute_clip_path> ()->clip_path ();
  const svg_attribute_visibility *visibility = get_computed_attribute<svg_attribute_visibility> ();
  const svg_attribute_display *display = get_computed_attribute<svg_attribute_display> ();
  const svg_attribute_opacity *opacity = get_computed_attribute<svg_attribute_opacity> ();

  // For now display does the same stuff as visibility, though they little differ by specification
  item->set_visibility (visibility->value () == visibility::VISIBLE && display->value () != display::NONE);
  item->set_transform (transform->computed_transform ());
  item->set_bounding_box (exact_bbox (false));
  item->set_opacity (opacity->value ());
  if (clip_path)
    item->set_clip_path (clip_path->get_clip_path ());
}

abstract_renderer_item *svg_graphics_item::create_renderer_item () const 
{
  renderer_graphics_item *render_item = create_renderer_graphics_item ();
  set_item_style (render_item);
  return render_item;
}

QRectF svg_graphics_item::exact_bbox (bool use_full_transform) const
{
  QPainterPath path = get_boundaries ();
  const svg_attribute_transform *transform = get_computed_attribute<svg_attribute_transform> ();

  if (use_full_transform)
    path = full_transform ().map (path);
  else
    path = transform->computed_transform ().map (path);
  return path.boundingRect ();
}

QRectF svg_graphics_item::bbox_with_clip (bool use_full_transform) const
{
  const svg_item_clip_path *clip_path = get_computed_attribute<svg_attribute_clip_path> ()->clip_path ();
  const svg_attribute_transform *transform = get_computed_attribute<svg_attribute_transform> ();
  QPainterPath path = get_boundaries ();

  if (clip_path)
    path = path.intersected (clip_path->get_clip_path ());

  if (use_full_transform)
    path = full_transform ().map (path);
  else
    path = transform->computed_transform ().map (path);
  return path.boundingRect ();
}
