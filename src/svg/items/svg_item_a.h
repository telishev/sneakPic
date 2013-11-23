#ifndef SVG_ITEM_A_H
#define SVG_ITEM_A_H

#include "svg/items/svg_item_group_type.h"

class svg_item_a : public svg_item_group_type
{
  SVG_ITEM

public:
  svg_item_a (svg_document *document);
  virtual ~svg_item_a () override;

  virtual abstract_renderer_item *create_renderer_item_impl () const override;
};

#endif // SVG_ITEM_A_H
