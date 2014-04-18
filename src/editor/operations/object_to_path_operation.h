#ifndef OBJECT_TO_PATH_OPERATION_H
#define OBJECT_TO_PATH_OPERATION_H


class svg_item_path;
class abstract_svg_item;

class object_to_path_operation
{
public:
  svg_item_path *apply (abstract_svg_item *item);
};

#endif // OBJECT_TO_PATH_OPERATION_H
