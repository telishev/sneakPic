#include "path/path_builder.h"

#include "path/svg_path.h"
#include "svg/svg_arc_data.h"


path_builder::path_builder (svg_path &dst_path)
  : m_dst_path (dst_path)
{
  m_prev_is_curve = false;
  m_prev_is_quad = false;
  if (!m_dst_path.empty ())
    {
      m_cur_position = m_dst_path.back ().back ().end_point;
      m_new_subpath_pending = false;
      m_prev_is_curve = true;
      m_prev_curve_c = m_dst_path.back ().back ().second_control;
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
  if (m_dst_path.empty () || m_dst_path.back ().empty ())
    return;

  m_dst_path.back ().set_closed (true);
  m_cur_position = m_dst_path.back ().front ().start_point;
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

  m_dst_path.back ().push_back (single_path_element::make_line (m_cur_position, dst));
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

void path_builder::quad_to (QPointF dst, QPointF c, bool relative)
{
  if (relative)
    {
      dst += m_cur_position;
      c += m_cur_position;
    }

  QPointF c1 = (m_cur_position + 2 * c) / 3;
  QPointF c2 = (dst + 2 * c) / 3;
  curve_to (dst, c1, c2, false);
  set_prev_quad_c (c); 
}

void path_builder::quad_to_short (QPointF dst, bool relative)
{
  if (!m_prev_is_quad)
    m_prev_quad_c = m_cur_position;

  if (relative)
    dst += m_cur_position;

  QPointF c = 2 * m_cur_position - m_prev_quad_c;
  quad_to (dst, c, false);
}

void path_builder::curve_to (QPointF dst, QPointF c1, QPointF c2, bool relative)
{
  check_new_subpath ();
  if (relative)
    {
      dst += m_cur_position;
      c1 += m_cur_position;
      c2 += m_cur_position;
    }

  m_dst_path.back ().push_back (single_path_element (m_cur_position, dst, c1, c2));
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
  curve_to (dst, c1, c2, false);
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
      m_dst_path.push_back (single_subpath ());
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
