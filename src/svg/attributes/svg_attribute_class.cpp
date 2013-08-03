#include "svg/attributes/svg_attribute_class.h"

#include "common/string_utils.h"

#include <QString>

svg_attribute_class::svg_attribute_class (abstract_svg_item *item)
  : abstract_attribute (item)
{

}

svg_attribute_class::~svg_attribute_class ()
{

}

bool svg_attribute_class::read (const char *data, bool /*from_css*/)
{
  trim_whitespace_left (data);
  while (*data)
    {
      m_class_names.push_back (get_next_identifier (data));
      trim_whitespace_left (data);
    }

  return m_class_names.size () != 0;
}

bool svg_attribute_class::write (QString &data, bool /*from_css*/) const 
{
  if (m_class_names.size () == 0)
    return false;

  std::string cur_data = m_class_names[0];
  for (int i = 1; i < m_class_names.size (); i++)
    {
      cur_data += " ";
      cur_data += m_class_names[1];
    }

  data = QString::fromStdString (cur_data);
  return true;
}

bool svg_attribute_class::is_class (const std::string &class_name) const
{
  return std::find (m_class_names.begin (), m_class_names.end (), class_name) != m_class_names.end ();
}
