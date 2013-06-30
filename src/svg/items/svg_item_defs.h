#ifndef SVG_ITEM_DEFS_H
#define SVG_ITEM_DEFS_H

#include "svg/items/abstract_svg_item.h"

class svg_item_defs : public abstract_svg_item
{
  SVG_ITEM
public:
  svg_item_defs (svg_document *document) : abstract_svg_item (document) {}
  virtual ~svg_item_defs () override {}

protected:
  virtual bool check_item () override { return true; }
};



#endif // SVG_ITEM_DEFS_H
