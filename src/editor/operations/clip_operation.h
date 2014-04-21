#ifndef CLIP_OPERATION_H
#define CLIP_OPERATION_H

class abstract_svg_item;

class clip_operation
{
public:
  void apply (abstract_svg_item *clip_item, const std::vector<abstract_svg_item *> &items);
};

#endif // CLIP_OPERATION_H
