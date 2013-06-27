#ifndef SVG_ITEM_GROUP_H
#define SVG_ITEM_GROUP_H

#include "svg/items/svg_named_item.h"

class svg_item_group : public svg_named_item
{
  SVG_ITEM ("g", svg_item_type::G, svg_namespaces_t::SVG)
public:
  svg_item_group (svg_document *document);
  virtual ~svg_item_group () override;

  virtual void draw (QPainter &painter) override;
};


#endif // SVG_ITEM_GROUP_H
