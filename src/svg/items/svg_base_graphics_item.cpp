#include "svg/items/svg_base_graphics_item.h"

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

svg_base_graphics_item::svg_base_graphics_item (svg_document *document)
  : svg_graphics_item (document)
{
}

svg_base_graphics_item::~svg_base_graphics_item ()
{
}

void svg_base_graphics_item::set_item_style (renderer_graphics_item *item) const
{
  const svg_item_clip_path *clip_path = get_computed_attribute<svg_attribute_clip_path> ()->clip_path (document ()->item_container ());
  const svg_attribute_visibility *attr_visibility = get_computed_attribute<svg_attribute_visibility> ();
  const svg_attribute_display *attr_display = get_computed_attribute<svg_attribute_display> ();
  const svg_attribute_opacity *opacity = get_computed_attribute<svg_attribute_opacity> ();

  // For now display does the same stuff as visibility, though they little differ by specification
  item->set_visibility (attr_visibility->value () == visibility::VISIBLE && attr_display->value () != display::NONE);
  item->set_transform (full_transform ());
  item->set_bounding_box (m_bbox);
  item->set_opacity (opacity->value ());
  if (clip_path)
    item->set_clip_path (clip_path->get_clip_path ());
}

abstract_renderer_item *svg_base_graphics_item::create_renderer_item_impl () const
{
  renderer_graphics_item *render_item = create_renderer_graphics_item ();
  if (!render_item)
    return 0;

  set_item_style (render_item);
  return render_item;
}

void svg_base_graphics_item::update_bbox_impl ()
{
  const svg_item_clip_path *clip_path = get_computed_attribute<svg_attribute_clip_path> ()->clip_path (document ()->item_container ());
  QPainterPath path = get_boundaries ();

  if (clip_path)
    path = path.intersected (clip_path->get_clip_path ());

  path = full_transform ().map (path);
  m_bbox = path.boundingRect ();
}

renderable_item * svg_base_graphics_item::create_outline_renderer () const
{
  QPainterPath path = get_transformed_boundaries ();
  renderer_overlay_path *overlay_item = new renderer_overlay_path;
  overlay_item->set_painter_path (path);
  return overlay_item;
}

QPainterPath svg_base_graphics_item::get_transformed_boundaries () const
{
  return full_transform ().map (get_boundaries ());
}
