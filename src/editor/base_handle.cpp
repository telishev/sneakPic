#include "editor/base_handle.h"

#include <QTransform>

#include "renderer/anchor_handle_renderer.h"
#include "path/geom_helpers.h"


base_handle::base_handle ()
{
  m_is_highlighted = false;
  m_is_selected = false;
  m_handle_type = handle_type::SQUARE;
}

base_handle::~base_handle ()
{

}

int base_handle::distance_to_mouse (QPoint screen_pos, QTransform transform) const 
{
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect element_rect = get_element_rect (transform);
  if (element_rect.contains (screen_pos))
    return 0;

  return geom::distance (center, screen_pos);
}

void base_handle::set_mouse_hovered (bool hovered)
{
  m_is_highlighted = hovered;
}

bool base_handle::start_drag (QPointF /*local_pos*/)
{
  return false;
}

bool base_handle::drag (QPointF /*local_pos*/)
{
  return false;
}

bool base_handle::end_drag (QPointF /*local_pos*/)
{
  return false;
}

void base_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  anchor_handle_renderer renderer (get_handle_center (), get_handle_type (), m_is_highlighted);
  renderer.set_is_selected (m_is_selected);
  renderer.draw (canvas, state, config);
}

handle_type base_handle::get_handle_type () const
{
  return m_handle_type;
}

QRect base_handle::get_element_rect (QTransform transform) const
{
  int mouse_size_px = anchor_handle_renderer::get_anchor_size_px ();
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect rect (0, 0, mouse_size_px, mouse_size_px);
  rect.moveCenter (center);
  return rect;
}
