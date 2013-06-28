#ifndef SVG_ITEM_PATH_H
#define SVG_ITEM_PATH_H

#include "svg/items/abstract_svg_item.h"

class svg_attribute_path_data;
class svg_attribute_style;

class svg_item_path : public abstract_svg_item
{
  SVG_ITEM

  svg_attribute_path_data *m_path_data;
  svg_attribute_style     *m_path_style;
public:
  svg_item_path (svg_document *document);
  virtual ~svg_item_path () override;

  virtual void draw (QPainter &painter) override;

protected:
  virtual void process_attribute (abstract_attribute *attribute) override;
};


#endif // SVG_ITEM_PATH_H
