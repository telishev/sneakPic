#ifndef ANCHOR_TYPE_CHANGE_OPERATION_H
#define ANCHOR_TYPE_CHANGE_OPERATION_H

class svg_path;
class svg_path_geom_iterator;
enum class node_type_t : char;

class anchor_type_change_operation
{
  svg_path *m_path;
public:
  anchor_type_change_operation (svg_path *path);
  ~anchor_type_change_operation ();

  void apply (svg_path_geom_iterator it, node_type_t node_type);
  void smooth_node (svg_path_geom_iterator it, bool symmetrize);
};

#endif // ANCHOR_TYPE_CHANGE_OPERATION_H
