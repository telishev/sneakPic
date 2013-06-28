#include "svg/items/svg_item_svg.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_version.h"
#include "svg/attributes/svg_attribute_width.h"
#include "svg/attributes/svg_attribute_height.h"

svg_item_svg::svg_item_svg (svg_document *document)
  : abstract_svg_item (document)
{
  m_version = nullptr;
  m_width = nullptr;
  m_height = nullptr;
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

  return abstract_svg_item::process_attribute (attribute);
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

