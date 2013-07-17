#include "svg/attributes/svg_attribute_offset.h"

#include <QString>

#include "common/common_utils.h"
#include "common/string_utils.h"


bool svg_attribute_offset::read (const char *data, bool /*from_css*/)
{
  CHECK (str_to_double (data, m_offset));
  trim_whitespace_left (data);
  if (*data == '%')
    m_offset *= 0.01;

  if (m_offset < 0.0)
    m_offset = 0.0;
  else if (m_offset > 1.0)
    m_offset = 1.0;

  return true;
}

bool svg_attribute_offset::write (QString &data, bool /*to_css*/ /*= false*/) const 
{
  data = double_to_str (m_offset);
  return true;
}
