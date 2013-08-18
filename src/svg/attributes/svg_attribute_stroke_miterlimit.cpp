#include "svg/attributes/svg_attribute_stroke_miterlimit.h"

#include "common/string_utils.h"

svg_attribute_stroke_miterlimit::svg_attribute_stroke_miterlimit (svg_document *document)
  : abstract_attribute (document)
{
  m_miterlimit = 4.0;
}

svg_attribute_stroke_miterlimit::~svg_attribute_stroke_miterlimit ()
{
}

bool svg_attribute_stroke_miterlimit::read (const char *data, bool /*from_css*/)
{
  bool ok = str_to_double (data, m_miterlimit);
  if (!ok) // According to svg specification mitterlimit < 1.0 should cause an error, for now let's ignore it
    m_miterlimit = 4.0;
  return true;
}

bool svg_attribute_stroke_miterlimit::write (QString &data, bool /*from_css*/) const
{
  data = double_to_str (m_miterlimit);
  return true;
}