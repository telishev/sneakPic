#ifndef SVG_ITEM_SVG_H
#define SVG_ITEM_SVG_H

#include "svg/items/svg_item_group_type.h"

class renderer_item_svg;

class svg_item_svg : public svg_item_group_type
{
  SVG_ITEM

public:
  svg_item_svg (svg_document *document);
  virtual ~svg_item_svg () override;

  double width () const;
  double height () const;

  virtual void update_renderer_item ();
  virtual abstract_renderer_item *create_renderer_item_impl () const;

protected:
  virtual bool check_item () override;
};

#endif // SVG_ITEM_SVG_H
