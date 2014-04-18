#ifndef STROKE_TO_PATH_OPERATION_H
#define STROKE_TO_PATH_OPERATION_H

class abstract_svg_item;

class stroke_to_path_operation
{
public:
  void apply (abstract_svg_item *item);

};


#endif // STROKE_TO_PATH_OPERATION_H
