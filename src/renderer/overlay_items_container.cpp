#include "renderer/overlay_items_container.h"

#include "renderer/overlay_renderer.h"
#include "renderer/abstract_renderer_item.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/svg_graphics_item.h"


overlay_items_container::overlay_items_container (svg_items_container *container)
{
  m_container = container;
}

overlay_items_container::~overlay_items_container ()
{
}

svg_items_container *overlay_items_container::svg_container () const
{
  return m_container;
}

void overlay_items_container::add_svg_item (const std::string &object)
{
  return svg_item_changed (object);
}

void overlay_items_container::svg_item_changed (const std::string &object)
{
  remove_svg_item (object);

  renderable_item *item = create_overlay_item (object);
  if (!item)
    return;

  m_obj_map[object] = item;
}

void overlay_items_container::remove_svg_item (const std::string &object)
{
  auto it = m_obj_map.find (object);
  if (it != m_obj_map.end ())
    {
      m_obj_map.erase (it);
    }
}

void overlay_items_container::clear_items ()
{
  m_obj_map.clear ();
}

renderable_item *overlay_items_container::create_overlay_for_item (const std::string &object, overlay_item_type overlay_type) const
{
  std::vector<abstract_renderer_item *> result;
  abstract_svg_item *svg_item = svg_container ()->get_item (object);
  if (!svg_item)
    return nullptr;

  svg_graphics_item *graphics_item = svg_item->to_graphics_item ();
  if (!graphics_item)
    return nullptr;

  return graphics_item->create_overlay_item (overlay_type);
}

void overlay_items_container::update_items ()
{
  auto m_map_copy = m_obj_map;
  for (const auto &item_pair : m_map_copy)
    svg_item_changed (item_pair.first);
}

void overlay_items_container::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  for (auto &item_pair : m_obj_map)
    item_pair.second->draw (canvas, state, config);
}
