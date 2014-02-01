#ifndef MERGE_PATH_OPERATION_H
#define MERGE_PATH_OPERATION_H

class svg_item_path;
class svg_path_geom_iterator;
class svg_path_geom;

class merge_path_operation
{
public:
  merge_path_operation ();
  ~merge_path_operation ();

  void apply (svg_item_path *path_dest, svg_path_geom_iterator dest, svg_item_path *path_src, svg_path_geom_iterator src);

private:
  void merge_inside (svg_item_path *path, svg_path_geom_iterator dest, svg_path_geom_iterator src);
  void merge_paths (svg_item_path *path_dest, svg_path_geom_iterator dest, svg_item_path *path_src, svg_path_geom_iterator src);

};

#endif // MERGE_PATH_OPERATION_H
