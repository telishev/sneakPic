#include "gradient_handles.h"

#include "renderer/renderer_items_gradient.h"
#include "path/geom_helpers.h"

#include "skia/skia_includes.h"
#include "renderer/qt2skia.h"
#include "renderer/path_preview_renderer.h"
#include "common/debug_utils.h"
#include "gradient_handles_editor.h"
#include "svg/items/abstract_svg_item.h"



QPointF base_gradient_handle::cur_point () const
{
  double coeff = m_gradient->stops ()[m_stop_num].first;
  return start_point () * (1 - coeff) + end_point () * coeff;
}

QTransform base_gradient_handle::gradient_transform () const
{
  return m_gradient->transform () * geom::rect2rect ({0, 0, 1, 1}, m_bbox);
}

handle_type base_gradient_handle::get_handle_type () const
{
  if (is_start ())
    return handle_type::SQUARE;
  else if (is_end ())
    return handle_type::SQUARE;
  else
    return handle_type::DIAMOND;
}

bool base_gradient_handle::is_end () const
{
  return m_stop_num == (int)m_gradient->stops ().size () - 1;
}

bool base_gradient_handle::is_start () const
{
  return m_stop_num == 0;
}

QPointF base_gradient_handle::end_point_local () const
{
  return  gradient_transform ().map (end_point ());
}

QPointF base_gradient_handle::start_point_local () const
{
  return  gradient_transform ().map (start_point ());
}

void base_gradient_handle::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  /// draw gradient line for first stop
  if (m_stop_num == 0)
    {
      SkPath path;
      path.moveTo (qt2skia::point (start_point_local ()));
      path.lineTo (qt2skia::point (end_point_local ()));
      simple_path_renderer renderer (path, QColor ("slateblue"));
      renderer.draw (canvas, state, config);
    }

  return base_handle::draw (canvas, state, config);
}

QPointF base_gradient_handle::get_handle_center () const
{
  return gradient_transform ().map (cur_point ());
}

void base_gradient_handle::interrupt_drag ()
{
  drag (m_drag_start, QTransform ());
}

bool base_gradient_handle::drag (QPointF local_pos, QTransform /*transform*/)
{
  QPointF gradient_pos = gradient_transform ().inverted ().map (local_pos);
  if (is_start ())
    move_start_point (gradient_pos);
  else if (is_end ())
    move_end_point (gradient_pos);
  else
    {
      double new_point = geom::projection_value (start_point_local (), end_point_local (), local_pos);
      m_gradient->stops ()[m_stop_num].first = new_point;
    }

  return true;
}

bool base_gradient_handle::start_drag (QPointF local_pos, QTransform /*transform*/)
{
  m_drag_start = local_pos;
  return true;
}

QColor base_gradient_handle::current_color () const
{
  return m_gradient->stops ()[m_stop_num].second;
}

base_gradient_handle::base_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num)
{
  m_handle = handle;
  m_stop_num = stop_num;
  m_bbox = bbox;
  m_gradient = gradient;
  set_color (Qt::white);
  set_handle_type (get_handle_type ());
}

bool base_gradient_handle::end_drag (QPointF local_pos, QTransform transform)
{
  drag (local_pos, transform);
  m_handle->apply_changes ();
  return true;
}

std::pair<std::string, int> base_gradient_handle::get_selection_id () const
{
  auto handles = m_handle->handles ();
  auto it = std::find (handles.begin (), handles.end (), this);
  DEBUG_ASSERT (it != handles.end ());

  return std::make_pair (m_handle->item ()->name (), (int)(it - handles.begin ()));
}

void base_gradient_handle::set_current_color (QColor color)
{
  m_gradient->stops ()[m_stop_num].second = color;
  m_handle->apply_changes ();
}

bool base_gradient_handle::remove ()
{
  if (is_start () || is_end ())
    return false;

  m_gradient->stops ().erase (m_gradient->stops ().begin () + m_stop_num);
  m_handle->apply_changes ();
  return true;
}

double base_gradient_handle::distance_to_line (QPointF pos, QTransform cur_transform) const
{
  return geom::distance (geom::projection (cur_transform.map (start_point_local ()), cur_transform.map (end_point_local ()), pos), pos);
}

inline static QColor mix_colors (QColor a, QColor b, double a_part)
{
  double b_part = 1. - a_part;
  QColor color = QColor::fromRgbF (a.redF ()   * a_part + b.redF ()   * b_part,
                                  a.greenF () * a_part + b.greenF () * b_part,
                                  a.blueF ()  * a_part + b.blueF ()  * b_part);
  color.setAlphaF (a.alphaF () * a_part + b.alphaF () * b_part);
  return color;
}

void base_gradient_handle::add_handle (QPointF local_pos)
{
  double value = geom::projection_value (start_point_local (), end_point_local (), local_pos);
  auto &stops = m_gradient->stops ();
  auto it = std::lower_bound (stops.begin (), stops.end (), value, 
                              [] (const std::pair<double, QColor> &lhs, double rhs) { return lhs.first < rhs; });
  
  if (it == stops.begin ())
    ++it;
  else if (it == stops.end ())
    --it;

  QColor color = mix_colors ((it - 1)->second, it->second, (it->first - value) / (it->first - (it - 1)->first));

  stops.insert (it, std::make_pair (value, color));
  m_handle->apply_changes ();
}

renderer_linear_gradient * linear_gradient_handle::gradient () const
{
  return static_cast<renderer_linear_gradient *> (m_gradient);
}

void linear_gradient_handle::move_end_point (QPointF dst)
{
  gradient ()->set_x2 (dst.x ()); gradient ()->set_y2 (dst.y ());
}

void linear_gradient_handle::move_start_point (QPointF dst)
{
  gradient ()->set_x1 (dst.x ()); gradient ()->set_y1 (dst.y ());
}

QPointF linear_gradient_handle::end_point () const
{
  return{gradient ()->x2 (), gradient ()->y2 ()};
}

QPointF linear_gradient_handle::start_point () const
{
  return{gradient ()->x1 (), gradient ()->y1 ()};
}

linear_gradient_handle::linear_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num)
  : base_gradient_handle (handle, gradient, bbox, stop_num)
{

}

renderer_radial_gradient * radial_gradient_handle::gradient () const
{
  return static_cast<renderer_radial_gradient *> (m_gradient);
}

void radial_gradient_handle::move_end_point (QPointF end_dst)
{
  QPointF src[3] = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
  QPointF dst[3] = {start_point (), get_end_point (true), get_end_point (false)};
  if (m_x_axis)
    dst[1] = end_dst;
  else
    dst[2] = end_dst;

  for (auto &point : dst)
    point = m_gradient->transform ().map (point);

  QTransform transform = geom::triangle2triangle (src, dst);
  gradient ()->set_transform (transform);
  gradient ()->set_cx (0);
  gradient ()->set_cy (0);
  gradient ()->set_r (1);
}

void radial_gradient_handle::move_start_point (QPointF dst)
{
  gradient ()->set_cx (dst.x ()); gradient ()->set_cy (dst.y ());
}

QPointF radial_gradient_handle::get_end_point (bool x_axis) const
{
  if (x_axis)
    return {gradient ()->cx () + gradient ()->r (), gradient ()->cy ()};
  else
    return {gradient ()->cx (), gradient ()->cy () + gradient ()->r ()};
}

QPointF radial_gradient_handle::end_point () const
{
  return get_end_point (m_x_axis);
}

QPointF radial_gradient_handle::start_point () const
{
  return {gradient ()->cx (), gradient ()->cy ()};
}

radial_gradient_handle::radial_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num, bool is_x_axis)
  : base_gradient_handle (handle, gradient, bbox, stop_num)
{
  m_x_axis = is_x_axis;
}
