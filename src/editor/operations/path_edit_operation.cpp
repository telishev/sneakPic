#include "path_edit_operation.h"

#include <QTransform>

#include "svg/items/svg_item_path.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/attribute_pointer.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "svg/attributes/svg_attribute_linetypes.h"

#include "path/svg_path_geom.h"


path_edit_operation::path_edit_operation (svg_item_path *path)
{
  m_path_item = path;
  m_path_data.reset (new attribute_pointer<svg_attribute_path_data> (m_path_item->get_attribute_for_change<svg_attribute_path_data> ()));
}

path_edit_operation::~path_edit_operation ()
{

}

void path_edit_operation::move_anchor (QPointF dst, svg_path_geom_iterator it)
{
  QTransform transform = m_path_item->full_transform ().inverted ();
  auto linetypes = m_path_item->get_computed_attribute<svg_attribute_linetypes> ();
  QPointF anchor_dst = transform.map (dst);
  QPointF diff = anchor_dst - it.anchor_point ();
  it.anchor_point () = anchor_dst;
  for (int i = 0; i < 2; i++)
    it.control_point (i == 0) += diff;

  /// check if left or right element is line and change control points correspondingly
  for (int i = 0; i < 2; i++)
    {
      bool left_segment = (i == 0);
      int element_id = it.segment_index (i == 0);
      if (   element_id < 0
          || !linetypes->is_line_segment (element_id))
        continue;

      it.control_point (left_segment) = anchor_dst;
      auto neighbour = it.neighbour (left_segment);
      neighbour.control_point (!left_segment) = neighbour.anchor_point ();
    }
  
}

void path_edit_operation::move_control_point (QPointF dst, svg_path_geom_iterator it, bool is_left)
{
  QTransform transform = m_path_item->full_transform ().inverted ();
  QPointF control_point = transform.map (dst);
  it.control_point (is_left) = control_point;

  auto nodetypes = m_path_item->get_computed_attribute<svg_attribute_nodetypes> ();
  node_type_t type = nodetypes->node_type (it.point_index ());
  switch (type)
    {
    case node_type_t::SYMMETRIC:
      {
        QPointF anchor = it.anchor_point ();
        it.control_point (!is_left) = 2 * anchor - control_point;
        break;
      }
    case node_type_t::SMOOTH:
      {
        QPointF anchor = it.anchor_point ();
        QPointF another_cp = it.control_point (!is_left);
        double length = geom::distance (another_cp, anchor);
        QPointF new_cp = anchor + length * geom::direction (control_point, anchor);
        it.control_point (!is_left) = new_cp;
        break;
      }

    case node_type_t::CUSP:
    case node_type_t::COUNT:
      return;
    }

}
