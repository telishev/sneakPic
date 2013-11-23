#ifndef SVG_ITEM_STYLE_H
#define SVG_ITEM_STYLE_H

#include "svg/items/abstract_svg_item.h"

#include <vector>

class selectors_container;

class svg_item_style : public abstract_svg_item
{
  SVG_ITEM

public:
  svg_item_style (svg_document *document);
  virtual ~svg_item_style () override;

  void add_style_to_container (selectors_container *container);

private:
  bool read_item (const char *data, selectors_container *container);

};

#endif // SVG_ITEM_STYLE_H
