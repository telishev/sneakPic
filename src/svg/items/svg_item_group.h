#ifndef SVG_ITEM_GROUP_H
#define SVG_ITEM_GROUP_H

#include "svg/items/abstract_svg_item.h"

class svg_item_group : public abstract_svg_item
{
  SVG_ITEM
public:
  svg_item_group (svg_document *document);
  virtual ~svg_item_group () override;

  virtual bool render_children () const { return true; }

protected:
  virtual bool check_item () override { return true; }
};


#endif // SVG_ITEM_GROUP_H
