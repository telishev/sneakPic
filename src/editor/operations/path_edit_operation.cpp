#include "path_edit_operation.h"

#include <QTransform>

#include "svg/items/svg_item_path.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/attribute_pointer.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "svg/attributes/svg_attribute_linetypes.h"

#include "path/svg_path.h"


path_edit_operation::path_edit_operation (svg_item_path *path)
{
  m_path_item = path;
  m_path_data.reset (new attribute_pointer<svg_attribute_path_data> (m_path_item->get_attribute_for_change<svg_attribute_path_data> ()));
}

path_edit_operation::~path_edit_operation ()
{

}

void path_edit_operation::move_anchor (QPointF dst, int anchor_id)
{
  QTransform transform = m_path_item->full_transform ().inverted ();
  svg_path *path = (*m_path_data)->path ();
  auto linetypes = m_path_item->get_computed_attribute<svg_attribute_linetypes> ();
  (*m_path_data)->path ()->move_point (anchor_id, transform.map (dst));

  /// check if left or right element is line and change control points correspondingly
  for (int i = 0; i < 2; i++)
    {
      int element_id = path->point_to_element (anchor_id, i == 0);
      if (   element_id < 0
          || !linetypes->is_line_segment (element_id))
        continue;

      single_path_element *element = path->element (element_id);
      *element = single_path_element::make_line (element->start, element->end);
    }
  
}

void path_edit_operation::move_control_point (QPointF dst, int control_id, bool is_left)
{
  QTransform transform = m_path_item->full_transform ().inverted ();
  svg_path *path = (*m_path_data)->path ();
  QPointF control_point = transform.map (dst);
  path->set_control_point (control_id, is_left, control_point);

  auto nodetypes = m_path_item->get_computed_attribute<svg_attribute_nodetypes> ();
  node_type_t type = nodetypes->node_type (control_id);
  switch (type)
    {
    case node_type_t::SYMMETRIC:
      {
        QPointF anchor = path->point (control_id);
        path->set_control_point (control_id, !is_left, 2 * anchor - control_point);
        break;
      }
    case node_type_t::SMOOTH:
      {
        QPointF anchor = path->point (control_id);
        QPointF another_cp = path->control_point (control_id, !is_left);
        double length = geom::distance (another_cp, anchor);
        QPointF new_cp = anchor + length * geom::direction (control_point, anchor);
        path->set_control_point (control_id, !is_left, new_cp);
        break;
      }

    case node_type_t::CUSP:
    case node_type_t::COUNT:
      return;
    }

}
