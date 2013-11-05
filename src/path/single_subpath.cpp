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
  return front ().start_point;
}

QPointF single_subpath::last_point () const
{
  return back ().end_point;
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
          back ().end_point = point;
          front ().start_point = point;
        }
      else
        front ().start_point = point;
    }
  else if (index == size ())
    {
      if (first_point_is_last ())
        {
          back ().end_point = point;
          front ().start_point = point;
        }
      else
        back ().end_point = point;
    }
  else
    {
      (*this)[index - 1].end_point = point;
      (*this)[index].start_point = point;
    }
}

QPointF single_subpath::point (size_t index) const
{
  if (index == 0)
    return front ().start_point;
  else
    return (*this)[index - 1].end_point;
}
