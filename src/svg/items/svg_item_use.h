#ifndef SVG_ITEM_USE_H
#define SVG_ITEM_USE_H

#include "svg/items/svg_item_group_type.h"

class svg_item_use : public svg_item_group_type
{
  SVG_ITEM

  renderer_item_group *m_renderer_item;
public:
  svg_item_use (svg_document *document);
  virtual ~svg_item_use () override;

  bool update_children_tree ();
  virtual void update_renderer_item () override;
  virtual const abstract_renderer_item *get_renderer_item () const override;

protected:
  virtual bool check_item () override;
};


#endif // SVG_ITEM_USE_H
