#ifndef GROUP_OBJECTS_OPERATION_H
#define GROUP_OBJECTS_OPERATION_H

class abstract_svg_item;
class svg_painter;

class group_objects_operation
{
  svg_painter *m_painter;
public:
  group_objects_operation (svg_painter *painter);

  abstract_svg_item *apply (const std::vector<abstract_svg_item *> &items);
};

#endif // GROUP_OBJECTS_OPERATION_H
