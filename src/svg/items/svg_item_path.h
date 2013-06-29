#ifndef SVG_ITEM_PATH_H
#define SVG_ITEM_PATH_H

#include "svg/items/abstract_svg_item.h"

class QPainterPath;

class svg_item_path : public abstract_svg_item
{
  SVG_ITEM

public:
  svg_item_path (svg_document *document);
  virtual ~svg_item_path () override;

  virtual void draw (QPainter &painter) override;

private:
  void draw_path (QPainter &painter, QPainterPath &painter_path);

};


#endif // SVG_ITEM_PATH_H
