#include "remove_anchors_operation.h"

#include "path/svg_path_geom.h"
#include "path/svg_path.h"
#include "svg/attributes/svg_attribute_nodetypes.h"

template<typename T, typename Func>
typename vector<T>::iterator remove_iter_if (vector<T> &vec, Func functor)
{
  typename vector<T>::iterator result = vec.begin (), first = vec.begin (), last = vec.end ();

  while (first != last)
    {
      if (!(functor (first - vec.begin ())))
        {
          *result = std::move (*first);
          ++result;
        }

      ++first;
    }

  return result;
}

remove_anchors_operation::remove_anchors_operation (svg_path *path)
{
  m_path = path;
}

remove_anchors_operation::~remove_anchors_operation ()
{

}

void remove_anchors_operation::apply (std::set<svg_path_geom_iterator> anchors_to_delete)
{
  remove_from_line_types (anchors_to_delete);
  remove_from_node_types (anchors_to_delete);
  remove_from_geom (anchors_to_delete);
}

void remove_anchors_operation::remove_from_node_types (const std::set<svg_path_geom_iterator> &anchors_to_delete)
{
  std::set<size_t> points_to_remove;
  auto &node_type = *m_path->get_node_type ();
  for (auto &&it : anchors_to_delete)
    {
      for (int i = 0; i < 2; i++)
        if (it.has_control_point (i == 0))
          node_type[it.neighbour (i == 0).point_index ()] = node_type_t::CUSP;
      points_to_remove.insert (it.point_index ());
    }

  node_type.erase (remove_iter_if (node_type,
    [&] (size_t index) { return points_to_remove.count (index) == 1; }), node_type.end ());
}

void remove_anchors_operation::remove_from_line_types (const std::set<svg_path_geom_iterator> &anchors_to_delete)
{
  std::set<size_t> segment_to_remove;
  auto &line_segments = *m_path->get_is_line_segment ();

  for (auto it = anchors_to_delete.begin (); it != anchors_to_delete.end (); ++it)
    {
      for (int i = 0; i < 2; i++)
        if (it->has_control_point (i == 0))
          line_segments[it->segment_index (i == 0)] = false;

      if (!it->has_control_point (false))
        segment_to_remove.insert (it->segment_index (true));
      else
        segment_to_remove.insert (it->segment_index (false));
    }

  line_segments.erase (remove_iter_if (line_segments,
    [&] (size_t index) { return segment_to_remove.count (index) == 1; }), line_segments.end ());
}

void remove_anchors_operation::remove_from_geom (const std::set<svg_path_geom_iterator> &anchors_to_delete)
{
  svg_path_geom &geom = *m_path->get_geom ();
  auto &subpath_vec = geom.subpath ();
  for (size_t i = 0; i < subpath_vec.size (); i++)
    {
      auto &subpath = subpath_vec[i];
      auto &elements = subpath.elements ();
      elements.erase (remove_iter_if (elements,
        [&] (size_t index) {return anchors_to_delete.count (svg_path_geom_iterator (geom, i, index)) == 1;}), elements.end ());
    }

  subpath_vec.erase (std::remove_if (subpath_vec.begin (), subpath_vec.end (), 
    [] (const single_subpath &subpath) { return subpath.total_points () == 0; }), subpath_vec.end ());
}
