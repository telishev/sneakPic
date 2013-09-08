#include "common/string_utils.h"
#include "svg/attributes/svg_attribute_font_size.h"

#include <QString>

svg_attribute_font_size::svg_attribute_font_size ()
{
  m_font_size.set_value (12.0); // as medium
}

svg_attribute_font_size::~svg_attribute_font_size ()
{
}

bool svg_attribute_font_size::read (const char *data, bool from_css)
{
  if (is_num (data[0]))
    {
      m_font_size.read (data, from_css);
      return true;
    }
  return false;
}

bool svg_attribute_font_size::write (QString &data, bool from_css) const
{
  data = m_font_size.write (from_css);
  return true;
}