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
