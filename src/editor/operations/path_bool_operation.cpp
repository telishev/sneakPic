#include "path_bool_operation.h"

#include "path_edit_operation.h"
#include "object_to_path_operation.h"
#include "svg/items/svg_item_path.h"
#include "path/qt_path_conversion.h"
#include "path/svg_path.h"
#include "common/debug_utils.h"




path_bool_operation::path_bool_operation (abstract_svg_item *dst, path_bool_operation_type type)
{
  m_type = type;
  m_dst = dst;
  m_dst_path = object_to_path_operation ().apply (dst);
  DEBUG_ASSERT (m_dst_path);
  put_in (m_edit_op, m_dst_path);
  put_in (m_dst_qpath, m_dst_path->get_path ());
}

path_bool_operation::~path_bool_operation ()
{
  qt_path_conversion ().create_from_qpath (*m_dst_qpath, *m_edit_op->get_svg_path ());
}

void path_bool_operation::apply (abstract_svg_item *other)
{
  svg_item_path *other_path = object_to_path_operation ().apply (other);
  if (!m_dst_path || !other_path)
    return;

  QPainterPath src_qpath = other_path->get_path ();
  switch (m_type)
  {
  case path_bool_operation_type::UNITE:
    *m_dst_qpath = m_dst_qpath->united (src_qpath); break;
  case path_bool_operation_type::INTERSECT:
    *m_dst_qpath = m_dst_qpath->intersected (src_qpath); break;
  case path_bool_operation_type::SUBTRACT:
    *m_dst_qpath = m_dst_qpath->subtracted (src_qpath); break;
  }

  other->parent ()->remove_child (other);
}
