#ifndef PATH_BOOL_OPERATION_H
#define PATH_BOOL_OPERATION_H

enum class path_bool_operation_type
{
  UNITE,
  INTERSECT,
  SUBTRACT,

};

class abstract_svg_item;
class svg_item_path;
class path_edit_operation;
class QPainterPath;

class path_bool_operation
{
  path_bool_operation_type m_type;
  abstract_svg_item *m_dst;
  svg_item_path *m_dst_path;
  unique_ptr<path_edit_operation> m_edit_op;
  unique_ptr<QPainterPath> m_dst_qpath;
public:
  path_bool_operation (abstract_svg_item *dst, path_bool_operation_type type);
  ~path_bool_operation ();

  void apply (abstract_svg_item *other);
  svg_item_path *dst_path () const { return m_dst_path; }
};

#endif // PATH_BOOL_OPERATION_H
