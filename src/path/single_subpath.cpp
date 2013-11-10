#include "path/single_subpath.h"

single_subpath::single_subpath ()
{
  m_is_closed = false;
}

single_subpath::~single_subpath ()
{

}

QPointF single_subpath::first_point () const
{
  return front ().start;
}

QPointF single_subpath::last_point () const
{
  return back ().end;
}

bool single_subpath::first_point_is_last () const
{
  return m_is_closed && first_point () == last_point ();
}

void single_subpath::set_point (size_t index, QPointF point)
{
  if (index == 0)
    {
      if (first_point_is_last ())
        {
          back ().end = point;
          front ().start = point;
        }
      else
        front ().start = point;
    }
  else if (index == size ())
    {
      if (first_point_is_last ())
        {
          back ().end = point;
          front ().start = point;
        }
      else
        back ().end = point;
    }
  else
    {
      (*this)[index - 1].end = point;
      (*this)[index].start = point;
    }
}

QPointF single_subpath::point (size_t index) const
{
  if (index == 0)
    return front ().start;
  else
    return (*this)[index - 1].end;
}

void single_subpath::apply_transform (const QTransform &transform)
{
  for (auto &element : *this)
    element.apply_transform (transform);
}

int single_subpath::total_handles () const
{
  if (first_point_is_last ())
    return (int)size ();
  else
    return (int)size () + 1;
}

QPointF single_subpath::control_point (size_t subpath_index, bool left_cp) const
{
  QPointF substitute;
  const QPointF *point = get_control_point (subpath_index, left_cp, substitute);
  return point ? *point : substitute;
}

QPointF * single_subpath::get_control_point (size_t subpath_index, bool left_cp, QPointF &substitute)
{
  if (subpath_index == 0)
    {
      if (!left_cp)
        return &front ().c1;
      else
        {
          if (first_point_is_last ())
            return &back ().c2;
          else
            {
              substitute = front ().start;
              return nullptr;
            }
        }
    }
  else if (subpath_index == size ())
    {
      if (left_cp)
        return &back ().c2;
      else
        {
          if (first_point_is_last ())
            return &front ().c1;
          else
            {
              substitute = back ().end;
              return nullptr;
            }
        }
    }
  else
    {
      if (left_cp)
        return &(*this)[subpath_index - 1].c2;
      else
        return &(*this)[subpath_index].c1;
    }
}

const QPointF *single_subpath::get_control_point (size_t subpath_index, bool left_cp, QPointF &substitute) const
{
  return const_cast<single_subpath *>(this)->get_control_point (subpath_index, left_cp, substitute);
}

void single_subpath::set_control_point (size_t subpath_index, bool left_cp, QPointF point)
{
  QPointF tmp;
  QPointF *result = get_control_point (subpath_index, left_cp, tmp);
  if (result)
    *result = point;
}

bool single_subpath::control_point_exists (size_t subpath_index, bool left_cp) const
{
  QPointF substitute;
  const QPointF *point = get_control_point (subpath_index, left_cp, substitute);
  return point != nullptr;
}
