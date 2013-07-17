#include "svg/css/type_selector.h"

#include "svg/items/abstract_svg_item.h"

type_selector::type_selector (const std::string &type)
{
  m_type = type;
}

type_selector::~type_selector ()
{

}

bool type_selector::is_matched (const abstract_svg_item *item) const 
{
  return item->type_name () == m_type;
}
