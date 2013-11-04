#include "path/svg_path.h"


svg_path::svg_path ()
{

}

svg_path::~svg_path ()
{

}

size_t svg_path::total_elements () const
{
  size_t total = 0;
  for (const auto &subpath : *this)
    total += subpath.size ();

  return total;
}

single_path_element *svg_path::element (size_t index)
{
  size_t total = 0;
  for (auto &subpath : *this)
    {
      size_t new_total = total + subpath.size ();
      if (new_total < index)
        {
          total = new_total;
          continue;
        }

      return &subpath[index - total];
    }

  return nullptr;
}

size_t svg_path::total_points () const
{
  size_t total = 0;
  for (const auto &subpath : *this)
    total += subpath.size () + 1;

  return total;
}

QPointF svg_path::point (size_t index) const
{
  size_t total = 0;
  for (auto &subpath : *this)
    {
      size_t new_total = total + subpath.size () + 1;
      if (new_total < index)
        {
          total = new_total;
          continue;
        }

      size_t index_in_subpath = index - total;
      if (index_in_subpath == 0)
        return subpath.front ().start_point;
      else
        return subpath[index_in_subpath - 1].end_point;
    }

  return QPointF ();
}

void svg_path::set_point (size_t index, QPointF point)
{
  size_t total = 0;
  for (auto &subpath : *this)
    {
      size_t new_total = total + subpath.size () + 1;
      if (new_total < index)
        {
          total = new_total;
          continue;
        }

      size_t index_in_subpath = index - total;
      if (index_in_subpath == 0)
        subpath.front ().start_point = point;
      else if (index_in_subpath == subpath.size ())
        subpath.back ().end_point = point;
      else
        {
          subpath[index_in_subpath - 1].end_point = point;
          subpath[index_in_subpath].start_point = point;
        }
    }
}
