#include "editor/operations/add_item_operation.h"
#include "apply_style_operation.h"
#include "renderer/svg_painter.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/svg_document.h"
#include "editor/items_selection.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/layers_handler.h"
#include "editor/style_controller.h"
#include "svg/attributes/svg_attributes_enum.h"

add_item_operation::add_item_operation (svg_painter *painter, bool clear_selection)
{
  m_painter = painter;
  m_clear_selection = clear_selection;
}

add_item_operation::~add_item_operation ()
{

}

bool add_item_operation::apply (abstract_svg_item *item)
{
  if (m_apply_style)
    apply_style_operation (m_painter->get_style_controller ()->active_style ()).apply (item);

  if (item->to_graphics_item ())
    item->to_graphics_item ()->update_bbox ();

  auto active_layer_item = m_painter->document ()->get_layers_handler ()->get_active_layer_item ();
  if (m_insert_item && active_layer_item->get_computed_attribute<svg_attribute_layer_type> ()->value () != layer_type::LAYER)
    return false;

  if (m_insert_item)
    active_layer_item->push_back (item);

  items_selection *selection = m_painter->selection ();
  if (m_clear_selection)
    selection->clear ();

  selection->add_item (item);
  return true;
}
