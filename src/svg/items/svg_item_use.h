#ifndef SVG_ITEM_USE_H
#define SVG_ITEM_USE_H

#include "svg/items/abstract_svg_item.h"

class svg_item_use : public abstract_svg_item
{
  SVG_ITEM
public:
  svg_item_use (svg_document *document);
  virtual ~svg_item_use () override;

  virtual bool render_children () const { return true; }

  bool update_children_tree ();

protected:
  virtual bool check_item () override;
};


#endif // SVG_ITEM_USE_H
