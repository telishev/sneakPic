#ifndef SVG_ITEM_GROUP_TYPE_H
#define SVG_ITEM_GROUP_TYPE_H

#include "svg/items/svg_graphics_item.h"

class renderer_item_group;

class svg_item_group_type : public svg_graphics_item
{
public:
  svg_item_group_type (svg_document *document);

  virtual abstract_renderer_item *create_overlay_item (overlay_item_type overlay_type) const override;
  virtual void update_bbox () override;

protected:
  void update_group_item (renderer_item_group *renderer_item) const;
};


#endif // SVG_ITEM_GROUP_TYPE_H
