#ifndef CLONE_ITEM_OPERATION_H
#define CLONE_ITEM_OPERATION_H

class abstract_svg_item;
class svg_item_use;
class svg_painter;

class clone_item_operation
{
  svg_painter *m_painter;
public:
  clone_item_operation (svg_painter *painter);

  svg_item_use *apply (abstract_svg_item *item);
};

#endif // CLONE_ITEM_OPERATION_H
