#ifndef PATH_EDIT_OPERATION_H
#define PATH_EDIT_OPERATION_H

#include <memory>

template <typename T>
class attribute_pointer;

class svg_attribute_path_data;
class svg_item_path;
class QPointF;

class path_edit_operation
{
  svg_item_path *m_path_item;
  std::unique_ptr<attribute_pointer<svg_attribute_path_data>> m_path_data;
public:
  path_edit_operation (svg_item_path *path);
  ~path_edit_operation ();

  void move_anchor (QPointF dst, int anchor_id);
  void move_control_point (QPointF dst, int control_id, bool is_left);
};

#endif // PATH_EDIT_OPERATION_H
