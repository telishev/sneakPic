#include "path/svg_path.h"
#include "common/debug_utils.h"


svg_path::svg_path ()
{}

svg_path::~svg_path ()
{}

size_t svg_path::total_segments () const
{
  size_t total = 0;
  for (const auto &subpath : m_subpath)
    total += subpath.total_segments ();

  return total;
}

size_t svg_path::total_points () const
{
  size_t total = 0;
  for (const auto &subpath : m_subpath)
    total += subpath.total_points ();

  return total;
}

void svg_path::apply_transform (const QTransform &transform)
{
  for (auto &cur_subpath : m_subpath)
    cur_subpath.apply_transform (transform);
}

svg_path_iterator svg_path::begin () const
{
  return svg_path_iterator (const_cast<svg_path &>(*this), 0, 0);
}

svg_path_iterator svg_path::end () const
{
  return svg_path_iterator ();
}

svg_path_iterator svg_path::last_point () const
{
  return svg_path_iterator (const_cast<svg_path &>(*this), m_subpath.size () - 1, m_subpath.back ().total_points () - 1);
}

void svg_path::clear ()
{
  m_subpath.clear ();
}



svg_path_iterator::svg_path_iterator ()
{
  m_path = 0;
  m_subpath_index = 0;
}

svg_path_iterator::svg_path_iterator (svg_path &path, size_t subpath_index, size_t subpath_point)
{
  m_path = &path;
  m_subpath_index = subpath_index;
  m_subpath_iterator = subpath_iterator (m_path->m_subpath[m_subpath_index], subpath_point);
}

svg_path_iterator::~svg_path_iterator ()
{}

svg_path_iterator &svg_path_iterator::operator++ ()
{
  if (!m_path)
    return *this;

  ++m_subpath_iterator;
  if (m_subpath_iterator.is_valid ())
    return *this;
  else if (m_subpath_index + 1 < m_path->m_subpath.size ())
    {
      m_subpath_index++;
      m_subpath_iterator = m_path->m_subpath[m_subpath_index].begin ();
      return *this;
    }

  *this = svg_path_iterator ();
  return *this;
}

bool svg_path_iterator::operator== (const svg_path_iterator &rhs) const
{
  return (m_path == rhs.m_path) && (m_subpath_index == rhs.m_subpath_index) && (m_subpath_iterator == rhs.m_subpath_iterator);
}

bool svg_path_iterator::operator!= (const svg_path_iterator &rhs) const
{
  return !(*this == rhs);
}

svg_path_iterator svg_path_iterator::neighbour (bool is_left) const
{
  return is_left ? left () : right ();
}

svg_path_iterator svg_path_iterator::left () const
{
  subpath_iterator it = m_subpath_iterator.left ();
  if (it.is_valid ())
    return svg_path_iterator (*m_path, m_subpath_index, it.point_num ());
  else
    return svg_path_iterator ();
}

svg_path_iterator svg_path_iterator::right () const
{
  subpath_iterator it = m_subpath_iterator.right ();
  if (it.is_valid ())
    return svg_path_iterator (*m_path, m_subpath_index, it.point_num ());
  else
    return svg_path_iterator ();
}

QPointF & svg_path_iterator::anchor_point ()
{
  return m_subpath_iterator.anchor_point ();
}

const QPointF & svg_path_iterator::anchor_point () const
{
  return m_subpath_iterator.anchor_point ();
}

QPointF & svg_path_iterator::control_point (bool is_left)
{
  return m_subpath_iterator.control_point (is_left);
}

const QPointF & svg_path_iterator::control_point (bool is_left) const
{
  return m_subpath_iterator.control_point (is_left);
}

bool svg_path_iterator::has_control_point (bool is_left)
{
  return m_subpath_iterator.has_control_point (is_left);
}

size_t svg_path_iterator::point_index () const
{
  size_t index = 0;
  for (size_t subpath_index = 0; subpath_index < m_subpath_index; subpath_index++)
    index += m_path->m_subpath[subpath_index].total_points ();

  return index + m_subpath_iterator.point_num ();
}

int svg_path_iterator::segment_index (bool is_left) const
{
  const single_subpath &subpath = m_path->m_subpath[m_subpath_index];
  size_t subpath_point = m_subpath_iterator.point_num ();
  if (   (is_left  && !subpath.is_closed () && subpath_point == 0)
      || (!is_left && subpath_point >= subpath.total_segments ()))
    return -1;

  size_t index = 0;
  for (size_t subpath_index = 0; subpath_index < m_subpath_index; subpath_index++)
    index += m_path->m_subpath[subpath_index].total_segments ();

  if (is_left)
    {
      if (subpath_point == 0)
        return (int)(index + subpath.total_segments () - 1);
      else
        return (int)(index + subpath_point - 1);
    }

  return (int)(index + subpath_point);
}

single_path_segment svg_path_iterator::segment (bool is_left) const
{
  svg_path_iterator left_it = *this, right_it = *this;
  if (is_left)
    left_it = left_it.left ();
  else
    right_it = right_it.right ();

  if (!left_it.is_valid () || !right_it.is_valid ())
    return single_path_segment ();

  return single_path_segment (left_it.anchor_point (), right_it.anchor_point (), left_it.control_point (false), right_it.control_point (true));
}

