#include "path/svg_path.h"
#include "common/debug_utils.h"


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
      if (new_total <= index)
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
  size_t subpath_index = -1;
  const single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  return subpath->point (subpath_index);
}

void svg_path::set_point (size_t index, QPointF point)
{
  size_t subpath_index = -1;
  single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  subpath->set_point (subpath_index, point);
}

void svg_path::get_subpath_and_index (size_t global_index, single_subpath *&subpath, size_t &index)
{
  size_t total = 0;
  for (auto &cur_subpath : *this)
    {
      size_t new_total = total + cur_subpath.size () + 1;
      if (new_total <= global_index)
        {
          total = new_total;
          continue;
        }

      index = global_index - total;
      subpath = &cur_subpath;
      return;
    }

  DEBUG_PAUSE ("wrong index");
}

void svg_path::get_subpath_and_index (size_t global_index, const single_subpath *&subpath, size_t &index) const
{
  single_subpath *sub;
  const_cast<svg_path *> (this)->get_subpath_and_index (global_index, sub, index);
  subpath = sub;
}
