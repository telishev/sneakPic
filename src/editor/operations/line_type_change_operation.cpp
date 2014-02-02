#include "line_type_change_operation.h"
#include "path/svg_path_geom.h"
#include "path/svg_path.h"




line_type_change_operation::line_type_change_operation (svg_path *path)
{
  m_path = path;
}

line_type_change_operation::~line_type_change_operation ()
{

}

void line_type_change_operation::apply (svg_path_geom_iterator it, bool is_left, bool is_line)
{
  if (!it.has_control_point (is_left))
    return;

  if ((*m_path->get_is_line_segment ())[it.segment_index (is_left)] == is_line)
    return;

  (*m_path->get_is_line_segment ())[it.segment_index (is_left)] = is_line;
  if (is_line)
    {
      it.control_point (is_left) = it.anchor_point ();
      it.neighbour (is_left).control_point (!is_left) = it.neighbour (is_left).anchor_point ();
    }
  else
    {
      it.control_point (is_left) = (2 * it.anchor_point () + it.neighbour (is_left).anchor_point ()) / 3;
      it.neighbour (is_left).control_point (!is_left) = (it.anchor_point () + 2 * it.neighbour (is_left).anchor_point ()) / 3;
    }
}
