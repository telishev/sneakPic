#include "item_helpers.h"
#include "svg/svg_document.h"
#include "svg/layers_handler.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_type.h"
#include "svg/attributes/svg_attributes_string.h"




item_helpers::item_helpers (svg_document *document)
{
  m_document = document;
}

const abstract_svg_item *item_helpers::get_selectable_item_or_group (const abstract_svg_item *item, selection_type_t type) const
{
  if (!item)
    return nullptr;

  while (!is_item_selectable (item, type))
    item = item->parent ();

  return item;
}

bool item_helpers::get_item_is_layer (const abstract_svg_item *item) const
{
  if (item->type () != svg_item_type::G)
    return false;

  return item->has_attribute (svg_attribute_layer_name::static_type_name ());
}

bool item_helpers::is_item_selectable (const abstract_svg_item *item, selection_type_t type) const
{
  if (type == selection_type_t::SELECT_ONLY_ITEMS)
    return item->to_base_graphics_item () != nullptr;

  layers_handler *handler = m_document->get_layers_handler ();
  const abstract_svg_item *current_layer = handler->get_active_layer_item (), *parent = item->parent ();
  if (parent && parent->parent () && !get_item_is_layer (parent) && parent != current_layer)
    return false;

  return true;
}
