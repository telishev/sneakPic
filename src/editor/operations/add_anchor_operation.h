#ifndef ADD_ANCHOR_OPERATION_H
#define ADD_ANCHOR_OPERATION_H

class svg_path;
class svg_path_geom_iterator;

class add_anchor_operation
{
  svg_path *m_path;
public:
  add_anchor_operation (svg_path *path);

  bool apply (svg_path_geom_iterator point_before, double position);
};

#endif // ADD_ANCHOR_OPERATION_H
