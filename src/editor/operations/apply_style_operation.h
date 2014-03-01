#ifndef APPLY_STYLE_OPERATION_H
#define APPLY_STYLE_OPERATION_H

class abstract_svg_item;
class item_paint_style;

class apply_style_operation
{
  const item_paint_style *m_style;
public:
  apply_style_operation (const item_paint_style *style);
  ~apply_style_operation ();

  void apply (abstract_svg_item *svg_item);

};

#endif // APPLY_STYLE_OPERATION_H
