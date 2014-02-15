#ifndef REMOVE_ANCHORS_OPERATION_H
#define REMOVE_ANCHORS_OPERATION_H

class svg_path;
class svg_path_geom_iterator;

class remove_anchors_operation
{
  svg_path *m_path;
public:
  remove_anchors_operation (svg_path *path);
  ~remove_anchors_operation ();

  void apply (std::set<svg_path_geom_iterator> anchors_to_delete);
private:
  void remove_from_node_types (const std::set<svg_path_geom_iterator> &anchors_to_delete);
  void remove_from_line_types (const std::set<svg_path_geom_iterator> &anchors_to_delete);
  void remove_from_geom (const std::set<svg_path_geom_iterator> &anchors_to_delete);

};

#endif // REMOVE_ANCHORS_OPERATION_H
