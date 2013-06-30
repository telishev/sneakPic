#ifndef SVG_ITEM_SVG_H
#define SVG_ITEM_SVG_H

#include "svg/items/abstract_svg_item.h"

#include <QString>

class svg_attribute_version;
class svg_attribute_width;
class svg_attribute_height;

class svg_item_svg : public abstract_svg_item
{
  SVG_ITEM

public:
  svg_item_svg (svg_document *document);
  virtual ~svg_item_svg () override;

  double width () const;
  double height () const;

  virtual void draw (QPainter &painter) override;

protected:
  virtual bool check_item () override { return true; }
};

#endif // SVG_ITEM_SVG_H
