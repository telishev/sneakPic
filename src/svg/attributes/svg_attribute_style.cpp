#include "svg/attributes/svg_attribute_style.h"

#include "common/memory_deallocation.h"

#include "svg/css/css_declaration.h"

#include "svg/items/abstract_svg_item.h"



svg_attribute_style::svg_attribute_style (abstract_svg_item *item)
  : abstract_attribute (item)
{
  declaration = new css_declaration (item);
}

svg_attribute_style::~svg_attribute_style ()
{
  FREE (declaration);
}

bool svg_attribute_style::read (const char *data, bool /*from_css*/)
{
  return declaration->parse (data);
}

bool svg_attribute_style::write (QString &data, bool /*to_css*/) const 
{
  data = QString::fromStdString (declaration->to_string ());
  return true;
}

abstract_attribute *svg_attribute_style::get_attribute (const std::string &str) const
{
  return declaration->get_attribute (str);
}
