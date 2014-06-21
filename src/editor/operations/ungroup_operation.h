#ifndef UNGROUP_OBJECTS_OPERATION_H
#define UNGROUP_OBJECTS_OPERATION_H

class svg_painter;
class abstract_svg_item;

class ungroup_operation
{
  svg_painter *m_painter;
public:
  ungroup_operation (svg_painter *painter);
  void apply (abstract_svg_item *group);
};

#endif // UNGROUP_OBJECTS_OPERATION_H
