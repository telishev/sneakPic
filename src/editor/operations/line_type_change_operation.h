#ifndef LINE_TYPE_CHANGE_OPERATION_H
#define LINE_TYPE_CHANGE_OPERATION_H

class svg_path;
class svg_path_geom_iterator;
enum class cp_type : char;

class line_type_change_operation
{
  svg_path *m_path;
public:
  line_type_change_operation (svg_path *path);
  ~line_type_change_operation ();

  void apply (svg_path_geom_iterator it, cp_type type, bool is_line);
};


#endif // LINE_TYPE_CHANGE_OPERATION_H
