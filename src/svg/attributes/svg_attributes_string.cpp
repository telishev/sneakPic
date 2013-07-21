#include "svg/attributes/svg_attributes_string.h"

#include <QString>

#include "common/string_utils.h"

#include "svg/items/abstract_svg_item.h"

bool svg_base_attribute_string::read (const char *data, bool /*from_css*/ /*= false*/)
{
  m_value = QString (data);
  return true;
}

bool svg_base_attribute_string::write (QString &data, bool /*from_css*/ /*= false*/) const 
{
  data = m_value;
  return true;
}
