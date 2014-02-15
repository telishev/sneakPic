#ifndef PATH_EDIT_OPERATION_H
#define PATH_EDIT_OPERATION_H

#include <memory>

template <typename T>
class attribute_pointer;

class svg_attribute_path_data;
class svg_attribute_linetypes;
class svg_attribute_nodetypes;
class svg_item_path;
class svg_path;

class path_edit_operation
{
  unique_ptr<attribute_pointer<svg_attribute_path_data>> m_path_data;
  unique_ptr<attribute_pointer<svg_attribute_linetypes>> m_linetypes;
  unique_ptr<attribute_pointer<svg_attribute_nodetypes>> m_nodetypes;
  unique_ptr<svg_path> m_svg_path;

public:
  path_edit_operation (svg_item_path *path);
  ~path_edit_operation ();

  svg_path *get_svg_path () const { return m_svg_path.get (); }
};

#endif // PATH_EDIT_OPERATION_H
