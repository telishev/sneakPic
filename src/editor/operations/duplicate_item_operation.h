#ifndef DUPLICATE_ITEM_OPERATION_H
#define DUPLICATE_ITEM_OPERATION_H

class svg_painter;
class abstract_svg_item;

class duplicate_item_operation
{
  svg_painter *m_painter;
  bool m_duplicate_clones;
public:
  duplicate_item_operation (svg_painter *painter = nullptr, bool duplicate_clones = false);

  abstract_svg_item *apply (abstract_svg_item *item);
};

#endif // DUPLICATE_ITEM_OPERATION_H
