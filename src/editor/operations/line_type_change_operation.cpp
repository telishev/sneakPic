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

void line_type_change_operation::apply (svg_path_geom_iterator it, cp_type type, bool is_line)
{
  if (!it.has_control_point (type))
    return;

  if ((*m_path->get_is_line_segment ())[it.segment_index (type)] == is_line)
    return;

  (*m_path->get_is_line_segment ())[it.segment_index (type)] = is_line;
  if (is_line)
    {
      it.control_point (type) = it.anchor_point ();
      it.neighbour (type).control_point (opposite (type)) = it.neighbour (type).anchor_point ();
    }
  else
    {
      it.control_point (type) = (2 * it.anchor_point () + it.neighbour (type).anchor_point ()) / 3;
      it.neighbour (type).control_point (opposite (type)) = (it.anchor_point () + 2 * it.neighbour (type).anchor_point ()) / 3;
    }
}
