#include "editor/base_handle.h"

#include <QTransform>

#include "renderer/anchor_handle_renderer.h"
#include "path/geom_helpers.h"


base_handle::base_handle ()
{
  m_handle_type = handle_type::SQUARE;
  m_renderer.set_visible (true);
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
  m_renderer.set_highlighted (hovered);
}

bool base_handle::start_drag (QPointF /*local_pos*/, QTransform /*transform*/)
{
  return false;
}

bool base_handle::drag (QPointF /*local_pos*/, QTransform /*transform*/, keyboard_modifier /*modifier*/)
{
  return false;
}

bool base_handle::end_drag (QPointF /*local_pos*/, QTransform /*transform*/, keyboard_modifier /*modifier*/)
{
  return false;
}

void base_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  if (!is_visible ())
    return;
  m_renderer.set_pos (get_handle_center ());
  m_renderer.set_node_type (get_handle_type ());
  m_renderer.set_anchor_size (get_handle_size ());
  m_renderer.set_rotation (handle_rotation ());
  m_renderer.draw (canvas, state, config);
}

handle_type base_handle::get_handle_type () const
{
  return m_handle_type;
}

QRect base_handle::get_element_rect (QTransform transform) const
{
  int mouse_size_px = m_renderer.get_anchor_size_px ();
  QPoint center = geom::nearest_point (transform.map (get_handle_center ()));
  QRect rect (0, 0, mouse_size_px, mouse_size_px);
  rect.moveCenter (center);
  return rect;
}

void base_handle::set_selected (bool selected)
{
  m_renderer.set_is_selected (selected);
}

void base_handle::set_highlighted (bool highlighted)
{
  m_renderer.set_highlighted (highlighted);
}

void base_handle::set_color (QColor color)
{
  m_renderer.set_color (color);
}

void base_handle::set_selected_color (QColor color)
{
  m_renderer.set_selected_color (color);
}

void base_handle::set_highlighted_color (QColor color)
{
  m_renderer.set_highlighted_color (color);
}

float base_handle::handle_rotation () const
{
  return 0.0f;
}

int base_handle::get_handle_size () const
{
  return 7;
}
