#ifndef SVG_ITEM_STOP_H
#define SVG_ITEM_STOP_H

#include "svg/items/abstract_svg_item.h"

class QColor;

class svg_item_stop : public abstract_svg_item
{
  SVG_ITEM
public:
  svg_item_stop (svg_document *document);
  virtual ~svg_item_stop () override;

  double offset () const;
  QColor color () const;
};


#endif // SVG_ITEM_STOP_H
