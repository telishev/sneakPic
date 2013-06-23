#ifndef SVG_ITEM_SVG_H
#define SVG_ITEM_SVG_H

#include "svg/items/svg_named_item.h"
#include "svg/svg_namespaces.h"

#include <QString>

class svg_attribute_version;
class svg_attribute_width;
class svg_attribute_height;

class svg_item_svg : public svg_named_item
{
  SVG_ITEM ("svg", svg_item_type::SVG, svg_namespaces_t::SVG)

  svg_attribute_version *version;
  svg_attribute_width   *width;
  svg_attribute_height  *height;
public:
  svg_item_svg (svg_document *document);
  virtual ~svg_item_svg () override;

protected:
  virtual void process_attribute (abstract_attribute *attribute) override;

  

};

#endif // SVG_ITEM_SVG_H
