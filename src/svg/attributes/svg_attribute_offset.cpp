#include "svg/attributes/svg_attribute_offset.h"

#include <QString>

#include "common/common_utils.h"
#include "common/string_utils.h"


bool svg_attribute_offset::read (const QString &data, bool /*from_css*/ /*= false*/)
{
  return read (data.toUtf8 ().constData ());
}

bool svg_attribute_offset::read (const char *data)
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
  data = QString::number (m_offset);
  return true;
}
