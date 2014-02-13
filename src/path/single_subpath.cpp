#include "path/single_subpath.h"

single_subpath::single_subpath ()
{
  m_is_closed = false;
}

single_subpath::~single_subpath ()
{

}

void single_subpath::apply_transform (const QTransform &transform)
{
  for (auto &element : m_elements)
    element.apply_transform (transform);
}

size_t single_subpath::total_points () const
{
  return m_elements.size ();
}

subpath_iterator single_subpath::begin () const
{
  return subpath_iterator (const_cast<single_subpath &> (*this), 0);
}


subpath_iterator single_subpath::end () const
{
  return subpath_iterator ();
}

subpath_iterator single_subpath::last_point () const
{
  return subpath_iterator (const_cast<single_subpath &> (*this), total_points () - 1);
}

size_t single_subpath::total_segments () const
{
  if (total_points () < 2)
    return 0;

  return m_is_closed ? total_points () : total_points () - 1;
}

void single_subpath::reverse ()
{
  m_elements = std::vector<single_path_point> (m_elements.rbegin (), m_elements.rend ());
  for (auto &elem : m_elements)
    std::swap (elem.c1, elem.c2);
}

void single_subpath::erase (size_t point_num)
{
  m_elements.erase (m_elements.begin () + point_num);
}

subpath_iterator::subpath_iterator (single_subpath &subpath, size_t point_num)
  : m_subpath (&subpath), m_point_num (point_num)
{}

subpath_iterator::subpath_iterator ()
{
  m_subpath = 0;
  m_point_num = 0;
}

subpath_iterator &subpath_iterator::operator++ ()
{
  if (!m_subpath)
    return *this;

  if (m_point_num + 1 < m_subpath->total_points ())
    m_point_num++;
  else
    *this = subpath_iterator ();

  return *this;
}


bool subpath_iterator::operator== (const subpath_iterator &rhs) const
{
  return m_subpath == rhs.m_subpath && m_point_num == rhs.m_point_num;
}

bool subpath_iterator::operator!= (const subpath_iterator &rhs) const
{
  return !(*this == rhs);
}

subpath_iterator subpath_iterator::neighbour (bool is_left) const
{
  return is_left ? left () : right ();
}

subpath_iterator subpath_iterator::left () const
{
  if (m_point_num != 0)
    return subpath_iterator ( *m_subpath, m_point_num - 1);
  else if (m_subpath->is_closed ())
    return subpath_iterator ( *m_subpath, m_subpath->total_points () - 1);

  return subpath_iterator ();
}

subpath_iterator subpath_iterator::right () const
{
  if (m_point_num != m_subpath->total_points () - 1)
    return subpath_iterator ( *m_subpath, m_point_num + 1);
  else if (m_subpath->is_closed ())
    return subpath_iterator ( *m_subpath, 0);

  return subpath_iterator ();
}

QPointF &subpath_iterator::anchor_point ()
{
  return m_subpath->m_elements[m_point_num].point;
}

const QPointF & subpath_iterator::anchor_point () const
{
  return const_cast<subpath_iterator *>(this)->anchor_point ();
}

QPointF &subpath_iterator::control_point (bool is_left)
{
  single_path_point &element = m_subpath->m_elements[m_point_num];
  return is_left ? element.c1 : element.c2;
}

const QPointF & subpath_iterator::control_point (bool is_left) const
{
  return const_cast<subpath_iterator *>(this)->control_point (is_left);
}

bool subpath_iterator::has_control_point (bool is_left)
{
  if (m_subpath->is_closed ())
    return true;

  if (is_left)
    return m_point_num != 0;
  else
    return m_point_num != m_subpath->total_points () - 1;
}

