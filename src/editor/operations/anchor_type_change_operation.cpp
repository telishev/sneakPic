#include "anchor_type_change_operation.h"

#include "path/svg_path_geom.h"
#include "path/svg_path.h"
#include "svg/attributes/svg_attribute_nodetypes.h"

#include <math.h>
#include "common/debug_utils.h"
#include "line_type_change_operation.h"


anchor_type_change_operation::anchor_type_change_operation (svg_path *path)
{
  m_path = path;
}

anchor_type_change_operation::~anchor_type_change_operation ()
{

}

void anchor_type_change_operation::apply (svg_path_geom_iterator it, node_type_t node_type)
{
  auto &node_type_array = *m_path->get_node_type ();
  size_t point_index = it.point_index ();
  if (node_type_array[point_index] == node_type)
    return;

  if (!it.has_control_point (cp_type::LEFT) || !it.has_control_point (cp_type::RIGHT))
    return;

  node_type_array[point_index] = node_type;
  switch (node_type)
    {
    case node_type_t::CUSP: return;
    case node_type_t::SMOOTH: return smooth_node (it, false);
    case node_type_t::SYMMETRIC: return smooth_node (it, true);
    case node_type_t::COUNT:return;
    }
}

void anchor_type_change_operation::smooth_node (svg_path_geom_iterator it, bool symmetrize)
{
  line_type_change_operation (m_path).apply (it, cp_type::LEFT, false);
  line_type_change_operation (m_path).apply (it, cp_type::RIGHT, false);

  QPointF left = it.control_point (cp_type::LEFT) - it.anchor_point ();
  QPointF right = it.control_point (cp_type::RIGHT) - it.anchor_point ();

  double angle_between = geom::angle_between (left, -right);
  double angle_left = geom::angle (left);
  double left_length = geom::norm (left);
  double right_length = geom::norm (right);

  double result_angle = angle_left - angle_between * left_length / (left_length + right_length);
  if (symmetrize)
    left_length = right_length = (right_length + left_length) / 2;

  it.control_point (cp_type::LEFT) = it.anchor_point () +  geom::vector_from_angle (result_angle, left_length);
  it.control_point (cp_type::RIGHT) = it.anchor_point () - geom::vector_from_angle (result_angle, right_length);
}


