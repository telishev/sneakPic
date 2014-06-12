#include "svg_path.h"

#include "svg_path_geom.h"
#include "svg/attributes/svg_attribute_nodetypes.h"

svg_path::svg_path (svg_path_geom *geom, vector<bool> *is_line_segment, vector<node_type_t> *node_type, QTransform transform)
{
  m_geom = geom;
  m_is_line_segment = is_line_segment;
  m_node_type = node_type;
  m_transform = transform;
}

svg_path::~svg_path ()
{
}

void svg_path::move_anchor (QPointF dst, svg_path_geom_iterator it)
{
  QTransform transform = m_transform.inverted ();
  QPointF anchor_dst = transform.map (dst);
  QPointF diff = anchor_dst - it.anchor_point ();
  it.anchor_point () = anchor_dst;
  for (int i = 0; i < 2; i++)
    it.control_point (i == 0 ? cp_type::LEFT : cp_type::RIGHT) += diff;

  /// check if left or right element is line and change control points correspondingly
  for (int i = 0; i < 2; i++)
    {
      bool left_segment = (i == 0);
      cp_type type = left_segment ? cp_type::LEFT : cp_type::RIGHT;
      int element_id = it.segment_index (i == 0 ? cp_type::LEFT : cp_type::RIGHT);
      if (   element_id < 0
          || !(*m_is_line_segment)[element_id])
        continue;

      it.control_point (type) = anchor_dst;
      auto neighbour = it.neighbour (type);
      neighbour.control_point (opposite (type)) = neighbour.anchor_point ();
    }

}

void svg_path::move_control_point (QPointF dst, svg_path_geom_iterator it, cp_type type)
{
  QTransform transform = m_transform.inverted ();
  QPointF control_point = transform.map (dst);
  it.control_point (type) = control_point;

  node_type_t node_type = (*m_node_type)[it.point_index ()];
  switch (node_type)
    {
    case node_type_t::SYMMETRIC:
      {
        QPointF anchor = it.anchor_point ();
        it.control_point (opposite (type)) = 2 * anchor - control_point;
        break;
      }
    case node_type_t::SMOOTH:
      {
        QPointF anchor = it.anchor_point ();
        QPointF another_cp = it.control_point (opposite (type));
        double length = geom::distance (another_cp, anchor);
        QPointF new_cp = anchor + length * geom::direction (control_point, anchor);
        it.control_point (opposite (type)) = new_cp;
        break;
      }

    case node_type_t::CUSP:
    case node_type_t::COUNT:
      return;
    }

}

void svg_path::reverse_subpath (int subpath_index)
{
  single_subpath &subpath = m_geom->subpath ()[subpath_index];
  subpath.reverse ();
  auto first_it = m_geom->subpath_begin (subpath_index);

  int segment_first = first_it.segment_index (cp_type::RIGHT);
  std::reverse (m_is_line_segment->begin () + segment_first, m_is_line_segment->begin () + segment_first + subpath.total_segments ());
  std::reverse (m_node_type->begin () + first_it.point_index (), m_node_type->begin () + first_it.point_index () + subpath.total_points ());
}

void svg_path::copy_from (svg_path &src)
{
  *m_geom = *src.m_geom;
  *m_is_line_segment = *src.m_is_line_segment;
  *m_node_type = *src.m_node_type;
  m_transform = src.m_transform;
}

void svg_path::clear ()
{
  m_geom->clear ();
  m_is_line_segment->clear ();
  m_node_type->clear ();
}

unique_svg_path::unique_svg_path ()
{
  m_geom.reset (new svg_path_geom);
  m_path.reset (new svg_path (m_geom.get (), &m_is_line_segment, &m_node_type, QTransform ()));
}

unique_svg_path::~unique_svg_path ()
{

}
