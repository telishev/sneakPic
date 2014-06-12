#include "remove_anchors_operation.h"

#include "path/svg_path_geom.h"
#include "path/svg_path.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "path/path_approximation.h"

template<typename T, typename Func>
typename vector<T>::iterator remove_iter_if (vector<T> &vec, Func functor)
{
  auto result = vec.begin (), first = vec.begin (), last = vec.end ();

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

template<typename T, typename Func, typename Apply>
void remove_iter_if_apply (vector<T> &vec, Func functor, Apply apply)
{
  auto prev_valid = vec.end ();
  bool has_valid = false;

  for (auto it = vec.begin (); it != vec.end (); ++it)
    {
      if (!(functor (it - vec.begin ())))
        {
          if (has_valid && it != prev_valid + 1)
            apply (prev_valid - vec.begin (), it - vec.begin ());

          has_valid = true;
          prev_valid = it;
        }
    }
}

remove_anchors_operation::remove_anchors_operation (svg_path *path)
{
  m_path = path;
}

remove_anchors_operation::~remove_anchors_operation ()
{

}

void remove_anchors_operation::apply (set<svg_path_geom_iterator> anchors_to_delete)
{
  remove_from_line_types (anchors_to_delete);
  remove_from_node_types (anchors_to_delete);
  remove_from_geom (anchors_to_delete);
}

void remove_anchors_operation::remove_from_node_types (const set<svg_path_geom_iterator> &anchors_to_delete)
{
  set<size_t> points_to_remove;
  auto &node_type = *m_path->get_node_type ();
  for (auto &&it : anchors_to_delete)
    {
      for (int i : {0, 1})
        if (it.has_control_point (i == 0 ? cp_type::LEFT : cp_type::RIGHT))
          node_type[it.neighbour (i == 0 ? cp_type::LEFT : cp_type::RIGHT).point_index ()] = node_type_t::CUSP;
      points_to_remove.insert (it.point_index ());
    }

  node_type.erase (remove_iter_if (node_type,
    [&] (size_t index) { return points_to_remove.count (index) == 1; }), node_type.end ());
}

void remove_anchors_operation::remove_from_line_types (const set<svg_path_geom_iterator> &anchors_to_delete)
{
  set<size_t> segment_to_remove;
  auto &line_segments = *m_path->get_is_line_segment ();

  for (auto it = anchors_to_delete.begin (); it != anchors_to_delete.end (); ++it)
    {
      for (int i = 0; i < 2; i++)
        if (it->has_control_point (i == 0 ? cp_type::LEFT : cp_type::RIGHT))
          line_segments[it->segment_index (i == 0 ? cp_type::LEFT : cp_type::RIGHT)] = false;

      if (!it->has_control_point (cp_type::RIGHT))
        segment_to_remove.insert (it->segment_index (cp_type::LEFT));
      else
        segment_to_remove.insert (it->segment_index (cp_type::RIGHT));
    }

  line_segments.erase (remove_iter_if (line_segments,
    [&] (size_t index) { return segment_to_remove.count (index) == 1; }), line_segments.end ());
}

static void approximate_for_deletion (std::vector<single_path_point> &elements, size_t first, size_t last)
{
  std::vector<QPointF> points;
  path_approximation approximator;
  for (size_t i = first; i < last; i++)
    {
      QPointF bezier[4];
      bezier[0] = elements[i].point;
      bezier[1] = elements[i].c2;
      bezier[2] = elements[i + 1].c1;
      bezier[3] = elements[i + 1].point;
      if (i == first)
        points.push_back (bezier[0]);

      for (double t = 0.25; t <= 1.0; t += 0.25)
        points.push_back (approximator.bezier_value (bezier, t));
    }
  QPointF approximation[4];
  approximator.fit_segment (points, approximation);

  elements[first].c2 = approximation[1];
  elements[last].c1 = approximation[2];

}

void remove_anchors_operation::remove_from_geom (const set<svg_path_geom_iterator> &anchors_to_delete)
{
  using namespace std::placeholders;
  svg_path_geom &geom = *m_path->get_geom ();
  auto &subpath_vec = geom.subpath ();
  for (size_t i = 0; i < subpath_vec.size (); i++)
    {
      auto &subpath = subpath_vec[i];
      auto &elements = subpath.elements ();
      auto need_to_del = [&] (size_t index) {return anchors_to_delete.count (svg_path_geom_iterator (geom, i, index)) == 1;};
      auto approximate = std::bind (approximate_for_deletion, std::ref (elements), _1, _2);

      remove_iter_if_apply (elements, need_to_del, approximate);
      elements.erase (remove_iter_if (elements, need_to_del), elements.end ());
    }

  subpath_vec.erase (std::remove_if (subpath_vec.begin (), subpath_vec.end (),
    [] (const single_subpath &subpath) { return subpath.total_points () == 0; }), subpath_vec.end ());
}
