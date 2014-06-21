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

abstract_svg_item *item_helpers::get_selectable_item_or_group (abstract_svg_item *item) const
{
  if (!item)
    return nullptr;

  layers_handler *handler = m_document->get_layers_handler ();
  abstract_svg_item *current_layer = handler->get_active_layer_item (), *parent = item->parent ();
  while (parent && parent->parent () && !get_item_is_layer (parent) && parent != current_layer)
    {
      item = parent;
      parent = item->parent ();
    }

  return item;
}

bool item_helpers::get_item_is_layer (abstract_svg_item *item) const
{
  if (item->type () != svg_item_type::G)
    return false;

  return item->has_attribute (svg_attribute_layer_name::static_type_name ());
}
