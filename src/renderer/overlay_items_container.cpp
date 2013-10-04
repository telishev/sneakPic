#include "renderer/overlay_items_container.h"

#include "renderer/overlay_renderer.h"
#include "renderer/abstract_renderer_item.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/svg_graphics_item.h"


overlay_items_container::overlay_items_container (overlay_renderer *overlay, overlay_layer_type layer_type)
{
  m_overlay = overlay;
  m_layer_type = layer_type;
}

overlay_items_container::~overlay_items_container ()
{
  clear_items ();
}

svg_items_container *overlay_items_container::svg_container () const
{
  return m_overlay->svg_container ();
}

void overlay_items_container::add_svg_item (const std::string &object)
{
  return svg_item_changed (object);
}

void overlay_items_container::svg_item_changed (const std::string &object)
{
  remove_svg_item (object);

  for (abstract_renderer_item *item : create_overlay_item (object))
    {
      m_obj_map.insert (std::make_pair (object, item->name ()));
      m_overlay->add_overlay_item (m_layer_type, item);
    }
}

void overlay_items_container::remove_svg_item (const std::string &object)
{
  auto it_pair = m_obj_map.equal_range (object);
  for (auto it = it_pair.first; it != it_pair.second; it++)
    m_overlay->remove_overlay_item (m_layer_type, it->second);

  m_obj_map.erase (it_pair.first, it_pair.second);
}

void overlay_items_container::clear_items ()
{
  for (auto it : m_obj_map)
    m_overlay->remove_overlay_item (m_layer_type, it.second);

  m_obj_map.clear ();
}

std::vector<abstract_renderer_item *> overlay_items_container::create_overlay_for_item (const std::string &object, overlay_item_type overlay_type) const
{
  std::vector<abstract_renderer_item *> result;
  abstract_svg_item *svg_item = svg_container ()->get_item (object);
  if (!svg_item)
    return result;

  svg_graphics_item *graphics_item = svg_item->to_graphics_item ();
  if (!graphics_item)
    return result;

  result.push_back (graphics_item->create_overlay_item (overlay_type));
  return result;
}

std::vector<abstract_renderer_item *> overlay_items_container::single_item_vector (abstract_renderer_item *item) const
{
  std::vector<abstract_renderer_item *> result;
  result.push_back (item);
  return result;
}

void overlay_items_container::update_items ()
{
  auto m_map_copy = m_obj_map;
  std::string cur_object;
  for (const auto &item_pair : m_map_copy)
    {
      if (item_pair.first == cur_object)
        continue;

      cur_object = item_pair.first;
      svg_item_changed (cur_object);
    }
}
