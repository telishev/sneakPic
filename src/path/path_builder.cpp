#include "path/path_builder.h"

#include "path/svg_path.h"
#include "path/svg_path_geom.h"
#include "svg/svg_arc_data.h"
#include "svg/attributes/svg_attribute_nodetypes.h"


path_builder::path_builder (svg_path &dst_path)
  : m_dst_path (dst_path)
{
  m_prev_is_curve = false;
  m_prev_is_quad = false;
  if (!m_dst_path.get_geom ()->m_subpath.empty ())
    {
      m_cur_position = m_dst_path.get_geom ()->m_subpath.back ().m_elements.back ().point;
      m_new_subpath_pending = false;
      m_prev_is_curve = true;
      m_prev_curve_c = m_dst_path.get_geom ()->m_subpath.back ().m_elements.back ().c1;
    }
  else
    m_new_subpath_pending = true;
}

path_builder::~path_builder ()
{

}

void path_builder::close_subpath ()
{
  m_new_subpath_pending = true;
  if (m_dst_path.get_geom ()->m_subpath.empty () || m_dst_path.get_geom ()->m_subpath.back ().m_elements.empty ())
    return;

  m_dst_path.get_geom ()->m_subpath.back ().set_closed (true);
  auto &elements = m_dst_path.get_geom ()->m_subpath.back ().m_elements;
  auto &front_elem = elements.front ();
  auto &back_elem = elements.back ();

  /// if first point equals last, remove it and move its first control point to first element
  if (elements.size () > 1 && front_elem.point == back_elem.point)
    {
      front_elem.c1 = back_elem.c1;
      elements.pop_back ();
      m_dst_path.get_is_line_segment ()->pop_back ();
      m_dst_path.get_node_type ()->pop_back ();
    }

  m_cur_position =  m_dst_path.get_geom ()->m_subpath.back ().m_elements.front ().point;
}

void path_builder::move_to (QPointF dst, bool relative)
{
  if (relative)
    dst += m_cur_position;

  m_cur_position = dst;
}

void path_builder::line_to (QPointF dst, bool relative)
{
  check_new_subpath ();
  if (relative)
    dst += m_cur_position;

  single_subpath &cur_subpath = m_dst_path.get_geom ()->m_subpath.back ();
  cur_subpath.m_elements.push_back (single_path_point (dst, dst, dst));
  m_dst_path.get_is_line_segment ()->push_back (true);
  m_dst_path.get_node_type ()->push_back (node_type_t::CUSP);
  m_cur_position = dst;
  clear_prev ();
}

void path_builder::hline_to (double x, bool relative)
{
  QPointF dst;
  if (!relative)
    dst = QPointF (x, m_cur_position.y ());
  else
    dst = QPointF (x + m_cur_position.x (), m_cur_position.y ());

  line_to (dst, false);
}

void path_builder::vline_to (double y, bool relative)
{
  QPointF dst;
  if (!relative)
    dst = QPointF (m_cur_position.x (), y);
  else
    dst = QPointF (m_cur_position.x (), m_cur_position.y () + y);

  line_to (dst, false);
}

void path_builder::quad_to (QPointF dst, QPointF c, bool relative, bool symmetrical)
{
  if (relative)
    {
      dst += m_cur_position;
      c += m_cur_position;
    }

  QPointF c1 = (m_cur_position + 2 * c) / 3;
  QPointF c2 = (dst + 2 * c) / 3;
  curve_to (dst, c1, c2, false, symmetrical);
  set_prev_quad_c (c); 
}

void path_builder::quad_to_short (QPointF dst, bool relative)
{
  if (!m_prev_is_quad)
    m_prev_quad_c = m_cur_position;

  if (relative)
    dst += m_cur_position;

  QPointF c = 2 * m_cur_position - m_prev_quad_c;
  quad_to (dst, c, false, false);
}

void path_builder::curve_to (QPointF dst, QPointF c1, QPointF c2, bool relative, bool symmetrical)
{
  check_new_subpath ();
  if (relative)
    {
      dst += m_cur_position;
      c1 += m_cur_position;
      c2 += m_cur_position;
    }

  single_subpath &cur_subpath = m_dst_path.get_geom ()->m_subpath.back ();
  single_path_point &last_point = cur_subpath.m_elements.back ();
  last_point.c2 = c1;
  m_dst_path.get_is_line_segment ()->push_back (false);
  /// do not change node if it is  first in subpath
  if (m_dst_path.get_geom ()->m_subpath.back ().total_points () > 1)
    m_dst_path.get_node_type ()->back () = symmetrical ? node_type_t::SYMMETRIC : node_type_t::SMOOTH;
  m_dst_path.get_node_type ()->push_back (node_type_t::CUSP);
  cur_subpath.m_elements.push_back (single_path_point (dst, c2, dst));
  m_cur_position = dst;
  set_prev_curve_c (c2);
}

void path_builder::curve_to_short (QPointF dst, QPointF c2, bool relative)
{
  if (!m_prev_is_curve)
    m_prev_curve_c = m_cur_position;

  if (relative)
    {
      dst += m_cur_position;
      c2 += m_cur_position;
    }

  QPointF c1 = 2 * m_cur_position - m_prev_curve_c;
  curve_to (dst, c1, c2, false, true);
}

void path_builder::arc_to (QPointF dst, double rx, double ry, double x_axis_rotation, int large_arc_flag, int sweep_flag, bool relative)
{
  if (relative)
    dst += m_cur_position;

  arc_converter::pathArc (*this, rx, ry, x_axis_rotation, large_arc_flag, sweep_flag, dst.x (), dst.y (), m_cur_position.x (), m_cur_position.y ());
}

void path_builder::check_new_subpath ()
{
  if (m_new_subpath_pending)
    {
      m_new_subpath_pending = false;
      single_subpath subpath;
      subpath.m_elements.push_back (single_path_point (m_cur_position, m_cur_position, m_cur_position));
      m_dst_path.get_geom ()->m_subpath.push_back (subpath);
      m_dst_path.get_node_type ()->push_back (node_type_t::CUSP);
    }
}

void path_builder::set_prev_quad_c (QPointF c)
{
  clear_prev ();
  m_prev_is_quad = true;
  m_prev_quad_c = c;
}

void path_builder::set_prev_curve_c (QPointF c)
{
  clear_prev ();
  m_prev_is_curve = true;
  m_prev_curve_c = c;
}

void path_builder::clear_prev ()
{
  m_prev_is_curve = false;
  m_prev_is_quad = false;
}

void path_builder::set_curve_c (QPointF c)
{
  m_prev_is_curve = true;
  m_prev_curve_c = 2 * m_cur_position - c;
  svg_path_geom_iterator last_point = m_dst_path.get_geom ()->last_point ();
  last_point.control_point (true) = m_prev_curve_c;
  last_point.control_point (false) = c;
}


