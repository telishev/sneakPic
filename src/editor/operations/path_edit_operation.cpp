#include "path_edit_operation.h"

#include <QTransform>

#include "svg/items/svg_item_path.h"
#include "svg/attributes/svg_attribute_path_data.h"
#include "svg/attributes/attribute_pointer.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "svg/attributes/svg_attribute_linetypes.h"

#include "path/svg_path_geom.h"
#include "path/svg_path.h"


path_edit_operation::path_edit_operation (svg_item_path *path)
{
  m_path_data.reset (new attribute_pointer<svg_attribute_path_data> (path->get_attribute_for_change<svg_attribute_path_data> ()));
  m_linetypes.reset (new attribute_pointer<svg_attribute_linetypes> (path->get_attribute_for_change<svg_attribute_linetypes> ()));
  m_nodetypes.reset (new attribute_pointer<svg_attribute_nodetypes> (path->get_attribute_for_change<svg_attribute_nodetypes> ()));

  QTransform full_transform = path->full_transform ();
  m_svg_path.reset (new svg_path ((*m_path_data)->path (), (*m_linetypes)->get_is_line_segment (), (*m_nodetypes)->get_node_type (), full_transform));
}

path_edit_operation::~path_edit_operation ()
{

}


