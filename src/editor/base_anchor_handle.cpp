#include "editor/base_anchor_handle.h"

#include <QTransform>

#include "renderer/anchor_handle_renderer.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "svg/items/svg_item_path.h"


base_anchor_handle::base_anchor_handle (svg_path_iterator path_it, svg_item_path *item)
{
  m_path_it = path_it;
  m_is_highlighted = false;
  m_node_type = node_type_t::SMOOTH;
  m_item = item;
}

base_anchor_handle::~base_anchor_handle ()
{

}

QPointF base_anchor_handle::get_handle_center () const
{
  QTransform transform = get_path_transform ();
  QPointF point = m_path_it.anchor_point ();
  return transform.map (point);
}

int base_anchor_handle::distance_to_mouse (QPoint screen_pos, QTransform transform) const 
{
  if (!m_path_it.is_valid ())
    return 1000;

  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect element_rect = get_element_rect (transform);
  if (element_rect.contains (screen_pos))
    return 0;

  return geom::distance (center, screen_pos);
}

void base_anchor_handle::set_mouse_hovered (bool hovered)
{
  m_is_highlighted = hovered;
}

bool base_anchor_handle::start_drag (QPointF /*local_pos*/)
{
  return false;
}

bool base_anchor_handle::drag (QPointF /*local_pos*/)
{
  return false;
}

bool base_anchor_handle::end_drag (QPointF /*local_pos*/)
{
  return false;
}

void base_anchor_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  if (!m_path_it.is_valid ())
    return;

  anchor_handle_renderer (get_handle_center (), node_type (), m_is_highlighted).draw (canvas, state, config);
}

node_type_t base_anchor_handle::node_type () const
{
  return m_node_type;
}

QRect base_anchor_handle::get_element_rect (QTransform transform) const
{
  int mouse_size_px = anchor_handle_renderer::get_anchor_size_px ();
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect rect (0, 0, mouse_size_px, mouse_size_px);
  rect.moveCenter (center);
  return rect;
}

QTransform base_anchor_handle::get_path_transform () const
{
  return m_item ? m_item->full_transform () : QTransform ();
}

void base_anchor_handle::set_path_iterator (svg_path_iterator path_it)
{
  m_path_it = path_it;
}
