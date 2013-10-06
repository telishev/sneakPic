#ifndef SVG_ITEM_GROUP_TYPE_H
#define SVG_ITEM_GROUP_TYPE_H

#include "svg/items/svg_graphics_item.h"

class renderer_item_group;

class svg_item_group_type : public svg_graphics_item
{
public:
  svg_item_group_type (svg_document *document);

  virtual void update_bbox_impl () override;

  virtual bool can_be_selected () const override { return false; }

protected:
  void update_group_item (renderer_item_group *renderer_item) const;
  virtual renderable_item *create_outline_renderer () const override;
};


#endif // SVG_ITEM_GROUP_TYPE_H
