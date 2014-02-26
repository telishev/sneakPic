#include "editor/items_move_handler.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"
#include "editor/operations/transform_item_operation.h"


#include "renderer/transformed_renderable_items.h"
#include "renderer/overlay_renderer.h"
#include "renderer/overlay_item_type.h"
#include "renderer/abstract_renderer_item.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_graphics_item.h"

#include "svg/svg_document.h"


items_move_handler::items_move_handler (svg_items_container *container, overlay_renderer *overlay_renderer, items_selection *selection, svg_document *document)
{
  m_overlay_renderer = overlay_renderer;
  m_selection = selection;
  m_document = document;
  m_transformed_items = new transformed_renderable_items;
  m_container = container;
  m_overlay_renderer->add_item (m_transformed_items, overlay_layer_type::TEMP);
}

items_move_handler::~items_move_handler ()
{
  FREE (m_transformed_items);
}

void items_move_handler::start_move (QPointF start_pos)
{
  m_cur_pos = m_start_pos = start_pos;
  for (const string &name : m_selection->selection ())
    {
      add_item (name);
    }
}

void items_move_handler::move (QPointF cur_pos)
{
  m_cur_pos = cur_pos;
  m_transformed_items->set_base_item_transform (current_transform ());
}

void items_move_handler::end_move ()
{
  m_transformed_items->clear_base_items ();
  apply_transform ();
}

void items_move_handler::add_item (const string &name)
{
  abstract_svg_item *item = m_container->get_editable_item (name);
  svg_graphics_item *graphics_item = item ? item->to_graphics_item () : nullptr;
  if (!graphics_item)
    return;

  renderable_item *overlay_item = graphics_item->create_overlay_item (overlay_item_type::CURRENT_ITEM);
  if (overlay_item)
    m_transformed_items->add_base_item (overlay_item);
}

QTransform items_move_handler::current_transform () const
{
  return QTransform::fromTranslate (m_cur_pos.x () - m_start_pos.x (), m_cur_pos.y () - m_start_pos.y ());
}

void items_move_handler::apply_transform ()
{
  transform_item_operation transform_operation (m_document);
  QTransform cur_transform = current_transform ();
  for (const string &name : m_selection->selection ())
    {
      abstract_svg_item *item = m_container->get_editable_item (name);
      if (!item)
        continue;

      transform_operation.apply_transform (cur_transform, item);
    }

  m_document->apply_changes ("Move");
}
