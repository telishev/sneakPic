#include "get_items_for_rect.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attribute_locked.h"
#include "svg/svg_document.h"


get_items_for_rect_operation::get_items_for_rect_operation (svg_document *doc)
{
  m_document = doc;
}

get_items_for_rect_operation::~get_items_for_rect_operation ()
{

}

struct items_for_rect_finder
{
  std::vector<const abstract_svg_item *> result;
  void add_items_for_rect_recursive (const QRectF &rect, const abstract_svg_item *root)
  {
    const svg_graphics_item *graphics_item = root->to_graphics_item ();
    if (!graphics_item)
      return;

    if (graphics_item->get_computed_attribute<svg_attribute_locked> ()->is_locked ())
      return;

    if (graphics_item->can_be_selected ())
      {
        if (!rect.contains (graphics_item->bbox ()))
          return;

        result.push_back (root);
        return;
      }

    if (!graphics_item->bbox ().intersects (rect))
      return;

    for (int i = 0; i < root->children_count (); i++)
      add_items_for_rect_recursive (rect, root->child (i));
  }

};

std::vector<const abstract_svg_item *> get_items_for_rect_operation::apply (QRectF rect)
{
  items_for_rect_finder finder;
  finder.add_items_for_rect_recursive (rect, m_document->root ());
  return std::move (finder.result);
}
