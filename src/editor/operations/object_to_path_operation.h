#ifndef OBJECT_TO_PATH_OPERATION_H
#define OBJECT_TO_PATH_OPERATION_H


class abstract_svg_item;

class object_to_path_operation
{
public:
  void apply (abstract_svg_item *item);
};

#endif // OBJECT_TO_PATH_OPERATION_H
