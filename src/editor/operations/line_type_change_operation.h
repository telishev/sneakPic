#ifndef LINE_TYPE_CHANGE_OPERATION_H
#define LINE_TYPE_CHANGE_OPERATION_H

class svg_path;
class svg_path_geom_iterator;

class line_type_change_operation
{
  svg_path *m_path;
public:
  line_type_change_operation (svg_path *path);
  ~line_type_change_operation ();

  void apply (svg_path_geom_iterator it, bool is_left, bool is_line);
};


#endif // LINE_TYPE_CHANGE_OPERATION_H
