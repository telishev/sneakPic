#ifndef RAISE_LOWER_OBJECTS_OPERATION_H
#define RAISE_LOWER_OBJECTS_OPERATION_H

class abstract_svg_item;

enum class z_direction
{
  UP,
  DOWN,
};

class raise_lower_objects_operation
{
public:
  raise_lower_objects_operation ();
  ~raise_lower_objects_operation ();

  void apply (const std::vector<abstract_svg_item *> &items, z_direction direction);
};

#endif // RAISE_LOWER_OBJECTS_OPERATION_H
