#include "editor/items_selection.h"

#include <QRectF>

#include "svg/items/abstract_svg_item.h"
#include "svg/svg_document.h"
#include "svg/items/svg_graphics_item.h"

#include "renderer/overlay_item_type.h"
#include "renderer/overlay_renderer.h"

items_selection::items_selection (overlay_renderer *overlay)
  : overlay_items_container (overlay, overlay_layer_type::BASE)
{
}

items_selection::~items_selection ()
{

}

void items_selection::add_item (const abstract_svg_item *item)
{
  return add_item (item->name ().toStdString ());
}

void items_selection::add_item (const std::string &item_name)
{
  m_selection.insert (item_name);
  add_svg_item (item_name);
}

void items_selection::clear ()
{
  m_selection.clear ();
  clear_items ();
}

void items_selection::add_items_for_rect (const QRectF &rect)
{
  add_items_for_rect (rect, document ()->root ());
}

void items_selection::add_items_for_rect (const QRectF &rect, const abstract_svg_item *root)
{
  const svg_graphics_item *graphics_item = root->to_graphics_item ();
  if (!graphics_item)
    return;

  if (graphics_item->can_be_selected ())
    {
      if (!rect.contains (graphics_item->bbox ()))
        return;

      add_item (root);
      return;
    }

  if (!graphics_item->bbox ().intersects (rect))
    return;

  for (const abstract_svg_item *child = root->first_child (); child; child = child->next_sibling ())
    add_items_for_rect (rect, child);
}

std::vector<abstract_renderer_item *> items_selection::create_overlay_item (const std::string &object) const 
{
  return create_overlay_for_item (object, overlay_item_type::SELECTION);
}

