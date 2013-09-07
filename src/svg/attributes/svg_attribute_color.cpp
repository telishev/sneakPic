#include "svg/attributes/svg_attribute_color.h"

#include "common/string_utils.h"


#include "svg/items/abstract_svg_item.h"

svg_attribute_color::svg_attribute_color (svg_document *document)
  : abstract_attribute (document)
{
}

svg_attribute_color::~svg_attribute_color ()
{
}

bool svg_attribute_color::read (const char *data, bool /*from_css*/)
{
  m_color = string_to_color (data);
  if (m_color.isValid ())
    return true;
  return false;
}

bool svg_attribute_color::write (QString &data, bool /*to_css*/) const 
{
  data = color_to_string (m_color);
  return true;
}
