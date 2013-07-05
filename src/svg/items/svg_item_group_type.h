#ifndef SVG_ITEM_GROUP_TYPE_H
#define SVG_ITEM_GROUP_TYPE_H

#include "svg/items/abstract_svg_item.h"

class renderer_item_group;

class svg_item_group_type : public abstract_svg_item
{
public:
  svg_item_group_type (svg_document *document);

protected:
  void update_group_item (renderer_item_group *renderer_item);
};


#endif // SVG_ITEM_GROUP_TYPE_H
