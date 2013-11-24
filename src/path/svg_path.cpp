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

const single_path_element *svg_path::element (size_t index) const
{
  return const_cast<svg_path *> (this)->element (index);
}

size_t svg_path::total_points () const
{
  size_t total = 0;
  for (const auto &subpath : *this)
    total += subpath.total_handles ();

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
      size_t new_total = total + cur_subpath.total_handles ();
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

void svg_path::apply_transform (const QTransform &transform)
{
  for (auto &cur_subpath : *this)
    cur_subpath.apply_transform (transform);
}

QPointF svg_path::control_point (size_t index, bool left_cp) const
{
  size_t subpath_index = -1;
  const single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  return subpath->control_point (subpath_index, left_cp);
}

void svg_path::set_control_point (size_t index, bool left_cp, QPointF point)
{
  size_t subpath_index = -1;
  single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  return subpath->set_control_point (subpath_index, left_cp, point);
}

bool svg_path::control_point_exists (size_t index, bool left_cp) const
{
  size_t subpath_index = -1;
  const single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  return subpath->control_point_exists (subpath_index, left_cp);
}

void svg_path::move_point (size_t index, QPointF new_point)
{
  QPointF old_point = point (index);
  QPointF diff = new_point - old_point;
  set_point (index, new_point);
  if (control_point_exists (index, false))
    set_control_point (index, false, control_point (index, false) + diff);

  if (control_point_exists (index, true))
    set_control_point (index, true, control_point (index, true) + diff);
}

size_t svg_path::prev_point (size_t index) const
{
  size_t subpath_index = -1;
  const single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  if (subpath_index == 0)
    return subpath->first_point_is_last () ? global_point_index (subpath->size () - 1, subpath) : index;
  else
    return index - 1;
}

size_t svg_path::next_point (size_t index) const
{
  size_t subpath_index = -1;
  const single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  if (subpath_index == subpath->size () - 1)
    return subpath->first_point_is_last () ? global_point_index (0, subpath) : index;
  else
    return index + 1;
}

size_t svg_path::global_point_index (size_t local_index, const single_subpath *subpath) const
{
  size_t total = 0;
  for (const auto &cur_subpath : *this)
    {
      if (&cur_subpath == subpath)
        break;

      total += cur_subpath.total_handles ();
    }

  return total + local_index;
}

int svg_path::point_to_element (size_t index, bool left_element) const
{
  size_t subpath_index = -1;
  const single_subpath *subpath = 0;
  get_subpath_and_index (index, subpath, subpath_index);
  if (left_element)
    {
      if (subpath_index != 0)
        return (int)global_element_index (subpath_index - 1, subpath);

      if (!subpath->is_closed ())
        return -1;

      return (int)global_element_index (subpath->size () - 1, subpath);
    }
  else
    {
      if (subpath_index != subpath->total_handles () - 1)
        return (int)global_element_index (subpath_index, subpath);

      if (!subpath->is_closed ())
        return -1;

      return (int)global_element_index (0, subpath);
    }
}

size_t svg_path::global_element_index (size_t local_index, const single_subpath *subpath) const
{
  size_t total = 0;
  for (const auto &cur_subpath : *this)
    {
      if (&cur_subpath == subpath)
        break;

      total += cur_subpath.size ();
    }

  return total + local_index;
}

