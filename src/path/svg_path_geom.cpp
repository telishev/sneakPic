#include "path/svg_path_geom.h"
#include "common/debug_utils.h"


svg_path_geom::svg_path_geom ()
{}

svg_path_geom::~svg_path_geom ()
{}

size_t svg_path_geom::total_segments () const
{
  size_t total = 0;
  for (const auto &subpath : m_subpath)
    total += subpath.total_segments ();

  return total;
}

size_t svg_path_geom::total_points () const
{
  size_t total = 0;
  for (const auto &subpath : m_subpath)
    total += subpath.total_points ();

  return total;
}

void svg_path_geom::apply_transform (const QTransform &transform)
{
  for (auto &cur_subpath : m_subpath)
    cur_subpath.apply_transform (transform);
}

svg_path_geom_iterator svg_path_geom::begin () const
{
  if (m_subpath.empty ())
    return end ();

  return svg_path_geom_iterator (const_cast<svg_path_geom &>(*this), 0, 0);
}

svg_path_geom_iterator svg_path_geom::end () const
{
  return svg_path_geom_iterator ();
}

svg_path_geom_iterator svg_path_geom::last_point () const
{
  return svg_path_geom_iterator (const_cast<svg_path_geom &>(*this), m_subpath.size () - 1, m_subpath.back ().total_points () - 1);
}

void svg_path_geom::clear ()
{
  m_subpath.clear ();
}

svg_path_geom_iterator svg_path_geom::subpath_begin (int subpath_index) const
{
  return svg_path_geom_iterator (const_cast<svg_path_geom &>(*this), subpath_index, m_subpath[subpath_index].begin ());
}

svg_path_geom_iterator svg_path_geom::subpath_last_point (int subpath_index) const
{
  return svg_path_geom_iterator (const_cast<svg_path_geom &>(*this), subpath_index, m_subpath[subpath_index].last_point ());
}

void svg_path_geom::erase (svg_path_geom_iterator iterator)
{
  iterator.subpath ().erase (iterator.get_subpath_point ());
}

svg_path_geom_iterator svg_path_geom::point (size_t point_index) const
{
  size_t subpath_sum = 0;
  for (size_t i = 0; i < m_subpath.size (); i++)
    {
      if (subpath_sum + m_subpath[i].total_points () < point_index)
        {
          subpath_sum += m_subpath[i].total_points ();
          continue;
        }

      return svg_path_geom_iterator (const_cast<svg_path_geom &>(*this), i, point_index - subpath_sum);
    }

  return end ();
}

svg_path_geom_iterator::svg_path_geom_iterator ()
{
  m_path = 0;
  m_subpath_index = 0;
}

svg_path_geom_iterator::svg_path_geom_iterator (svg_path_geom &path, size_t subpath_index, size_t subpath_point)
{
  m_path = &path;
  m_subpath_index = subpath_index;
  m_subpath_iterator = subpath_iterator (m_path->m_subpath[m_subpath_index], subpath_point);
}

svg_path_geom_iterator::svg_path_geom_iterator (svg_path_geom &path, size_t subpath_index, subpath_iterator iterator)
{
  m_path = &path;
  m_subpath_index = subpath_index;
  m_subpath_iterator = iterator;
}

svg_path_geom_iterator::~svg_path_geom_iterator ()
{}

svg_path_geom_iterator &svg_path_geom_iterator::operator++ ()
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

  *this = svg_path_geom_iterator ();
  return *this;
}

bool svg_path_geom_iterator::operator== (const svg_path_geom_iterator &rhs) const
{
  return (m_path == rhs.m_path) && (m_subpath_index == rhs.m_subpath_index) && (m_subpath_iterator == rhs.m_subpath_iterator);
}

bool svg_path_geom_iterator::operator!= (const svg_path_geom_iterator &rhs) const
{
  return !(*this == rhs);
}

svg_path_geom_iterator svg_path_geom_iterator::neighbour (cp_type type) const
{
  switch (type)
    {
    case cp_type::LEFT:
      return left ();
    case cp_type::RIGHT:
      return right ();
    }
  return {};
}

svg_path_geom_iterator svg_path_geom_iterator::left () const
{
  subpath_iterator it = m_subpath_iterator.left ();
  if (it.is_valid ())
    return svg_path_geom_iterator (*m_path, m_subpath_index, it.point_num ());
  else
    return svg_path_geom_iterator ();
}

svg_path_geom_iterator svg_path_geom_iterator::right () const
{
  subpath_iterator it = m_subpath_iterator.right ();
  if (it.is_valid ())
    return svg_path_geom_iterator (*m_path, m_subpath_index, it.point_num ());
  else
    return svg_path_geom_iterator ();
}

QPointF & svg_path_geom_iterator::anchor_point ()
{
  return m_subpath_iterator.anchor_point ();
}

const QPointF & svg_path_geom_iterator::anchor_point () const
{
  return m_subpath_iterator.anchor_point ();
}

QPointF & svg_path_geom_iterator::control_point (cp_type type)
{
  return m_subpath_iterator.control_point (type);
}

const QPointF & svg_path_geom_iterator::control_point (cp_type type) const
{
  return m_subpath_iterator.control_point (type);
}

bool svg_path_geom_iterator::has_control_point (cp_type type) const
{
  return m_subpath_iterator.has_control_point (type);
}

size_t svg_path_geom_iterator::point_index () const
{
  size_t index = 0;
  for (size_t subpath_index = 0; subpath_index < m_subpath_index; subpath_index++)
    index += m_path->m_subpath[subpath_index].total_points ();

  return index + m_subpath_iterator.point_num ();
}

int svg_path_geom_iterator::segment_index (cp_type type) const
{
  const single_subpath &subpath = m_path->m_subpath[m_subpath_index];
  size_t subpath_point = m_subpath_iterator.point_num ();
  if (   (type == cp_type::LEFT  && !subpath.is_closed () && subpath_point == 0)
      || (type == cp_type::RIGHT && subpath_point >= subpath.total_segments ()))
    return -1;

  size_t index = 0;
  for (size_t subpath_index = 0; subpath_index < m_subpath_index; subpath_index++)
    index += m_path->m_subpath[subpath_index].total_segments ();

  if (type == cp_type::LEFT)
    {
      if (subpath_point == 0)
        return (int)(index + subpath.total_segments () - 1);
      else
        return (int)(index + subpath_point - 1);
    }

  return (int)(index + subpath_point);
}

single_path_segment svg_path_geom_iterator::segment (cp_type type) const
{
  svg_path_geom_iterator left_it = *this, right_it = *this;

  switch (type)
    {
    case cp_type::LEFT:
      left_it = left_it.left ();
      break;
    case cp_type::RIGHT:
      right_it = right_it.right ();
      break;
    }

  if (!left_it.is_valid () || !right_it.is_valid ())
    return single_path_segment ();

  return single_path_segment (left_it.anchor_point (), right_it.anchor_point (), left_it.control_point (cp_type::RIGHT), right_it.control_point (cp_type::LEFT));
}

single_subpath & svg_path_geom_iterator::subpath ()
{
  return m_path->m_subpath[m_subpath_index];
}

size_t svg_path_geom_iterator::get_subpath_index () const
{
  return m_subpath_index;
}

size_t svg_path_geom_iterator::get_subpath_point () const
{
  return m_subpath_iterator.point_num ();
}

bool svg_path_geom_iterator::operator< (const svg_path_geom_iterator &rhs) const
{
  return std::make_pair (m_subpath_index, get_subpath_point ()) < std::make_pair (rhs.m_subpath_index, rhs.get_subpath_point ());
}

