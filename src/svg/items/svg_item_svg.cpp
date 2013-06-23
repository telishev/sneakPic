#include "svg/items/svg_item_svg.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_version.h"
#include "svg/attributes/svg_attribute_width.h"
#include "svg/attributes/svg_attribute_height.h"

svg_item_svg::svg_item_svg (svg_document *document)
  : svg_named_item (document)
{
  version = 0;
  width = 0;
  height = 0;
}

svg_item_svg::~svg_item_svg ()
{

}

void svg_item_svg::process_attribute (abstract_attribute *attribute)
{
  switch (attribute->type ())
    {
    case svg_attribute_type::VERSION:
      version = static_cast<svg_attribute_version *> (attribute);
      break;
    case svg_attribute_type::WIDTH:
      width = static_cast<svg_attribute_width *> (attribute);
      break;
    case svg_attribute_type::HEIGHT:
      height = static_cast<svg_attribute_height *> (attribute);
      break;
    default:
      break;
    }

  return svg_named_item::process_attribute (attribute);
}

