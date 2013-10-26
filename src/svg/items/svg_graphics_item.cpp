#include "svg_graphics_item.h"

#include <QTransform>

#include "svg/attributes/svg_attribute_transform.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/overlay_item_type.h"
#include "renderer/renderer_item_selection.h"


svg_graphics_item::svg_graphics_item (svg_document *document)
  : abstract_svg_item (document)
{
  m_bbox_valid = false;
}

svg_graphics_item::~svg_graphics_item ()
{

}

QTransform svg_graphics_item::full_transform () const
{
  QTransform total_transform;
  for (const abstract_svg_item *cur_item = this; cur_item; cur_item = cur_item->parent ())
    {
      const svg_attribute_transform *base_transform = cur_item->get_computed_attribute <svg_attribute_transform> ();
      total_transform = total_transform * base_transform->computed_transform ();
    }

  return total_transform;
}

void svg_graphics_item::update_bbox ()
{
  if (m_bbox_valid)
    return;

  update_bbox_impl ();
  m_bbox_valid = true;
}

void svg_graphics_item::invalidate_bbox ()
{
  for (abstract_svg_item *cur_item = this; cur_item; cur_item = cur_item->parent ())
    {
      svg_graphics_item *cur_graphics_item = cur_item->to_graphics_item ();
      if (!cur_graphics_item)
        continue;

      cur_graphics_item->m_bbox_valid = false;
    }
}

abstract_renderer_item *svg_graphics_item::create_renderer_item () const
{
  abstract_renderer_item *item = create_renderer_item_impl ();
  if (parent () && item)
    item->set_parent (parent ()->name ());

  return item;
}

renderable_item *svg_graphics_item::create_overlay_item (overlay_item_type overlay_type) const
{
  switch (overlay_type)
    {
    case overlay_item_type::CURRENT_ITEM:
      {
        return create_outline_renderer ();
      }
    case overlay_item_type::SELECTION:
      {
        renderer_item_selection *selection = new renderer_item_selection;
        selection->set_bbox (m_bbox);
        return selection;
      }
    case overlay_item_type::HANDLES:
      break;
    }
  return nullptr;
}
