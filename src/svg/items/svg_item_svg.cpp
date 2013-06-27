#include "svg/items/svg_item_svg.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_version.h"
#include "svg/attributes/svg_attribute_width.h"
#include "svg/attributes/svg_attribute_height.h"

svg_item_svg::svg_item_svg (svg_document *document)
  : svg_named_item (document)
{
  m_version = 0;
  m_width = 0;
  m_height = 0;
}

svg_item_svg::~svg_item_svg ()
{

}

void svg_item_svg::process_attribute (abstract_attribute *attribute)
{
  switch (attribute->type ())
    {
    case svg_attribute_type::VERSION:
      m_version = static_cast<svg_attribute_version *> (attribute);
      break;
    case svg_attribute_type::WIDTH:
      m_width = static_cast<svg_attribute_width *> (attribute);
      break;
    case svg_attribute_type::HEIGHT:
      m_height = static_cast<svg_attribute_height *> (attribute);
      break;
    default:
      break;
    }

  return svg_named_item::process_attribute (attribute);
}

double svg_item_svg::width () const
{
  return m_width->width ();
}

double svg_item_svg::height () const
{
  return m_height->height ();
}

void svg_item_svg::draw (QPainter &painter)
{
  for (abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    child->draw (painter);
}

