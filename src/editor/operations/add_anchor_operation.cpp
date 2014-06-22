#include "add_anchor_operation.h"
#include "path/svg_path.h"
#include "path/svg_path_geom.h"
#include "editor/tools/bezier_helpers.h"
#include "svg/attributes/svg_attribute_nodetypes.h"

add_anchor_operation::add_anchor_operation (svg_path *path)
{
  m_path = path;
}

bool add_anchor_operation::apply (svg_path_geom_iterator point_before, double position)
{
  if (!point_before.has_control_point (cp_type::RIGHT))
    return false;

  single_path_segment segment = point_before.segment (cp_type::RIGHT);
  auto splitted = bezier_helpers ().split (segment, position);

  auto &&is_line_segment = *m_path->get_is_line_segment ();
  auto && node_type = *m_path->get_node_type ();
  auto &&elements = point_before.subpath ().elements ();
  int segment_index = point_before.segment_index (cp_type::RIGHT);
  size_t point_index = point_before.point_index ();

  bool is_line = is_line_segment[segment_index];
  node_type_t type = is_line ? node_type_t::CUSP : node_type_t::SYMMETRIC;
  is_line_segment.insert (is_line_segment.begin () + segment_index + 1, is_line);
  node_type.insert (node_type.begin () + point_index + 1, type);

  size_t subpath_index = point_before.get_subpath_point ();
  elements[subpath_index].c2 = splitted[0].c1;
  elements[subpath_index + 1].c1 = splitted[1].c2;
  single_path_point new_point (splitted[0].end, splitted[0].c2, splitted[1].c1);

  elements.insert (elements.begin () + subpath_index + 1, new_point);
  return true;
}
