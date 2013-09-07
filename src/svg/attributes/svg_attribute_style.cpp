#include "svg/attributes/svg_attribute_style.h"

#include "common/memory_deallocation.h"

#include "svg/css/css_declaration.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/svg_document.h"



svg_attribute_style::svg_attribute_style (svg_document *document)
  : abstract_attribute (document)
{
  declaration = nullptr;
}

svg_attribute_style::~svg_attribute_style ()
{
  FREE (declaration);
}

bool svg_attribute_style::read (const char *data, bool /*from_css*/)
{
  FREE (declaration);
  declaration = new css_declaration (document ()->attribute_factory (), item_id ());
  return declaration->parse (data);
}

bool svg_attribute_style::write (QString &data, bool /*to_css*/) const 
{
  if (declaration)
    data = QString::fromStdString (declaration->to_string ());
  return true;
}

abstract_attribute *svg_attribute_style::get_attribute (const std::string &str) const
{
  if (declaration)
    return declaration->get_attribute (str);
  else
    return nullptr;
}
