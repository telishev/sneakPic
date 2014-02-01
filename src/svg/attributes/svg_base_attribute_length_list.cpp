#include "common/common_utils.h"

#include "svg/attributes/svg_base_attribute_length_list.h"

void svg_base_attribute_length_list::fill_values (QList<double> &list) const
{
  list.clear ();
  for (int i = 0; i < m_value.size (); i++)
    list.push_back (m_value[i].value (item (), m_units_orientation));
}

QList <double> svg_base_attribute_length_list::value () const
{
  QList<double> list;
  fill_values (list);
  return list;
}

bool svg_base_attribute_length_list::read (const char *data, bool from_css)
{
  m_value.clear ();
  while (*data)
    {
      svg_data_type_length single_value;
      CHECK (single_value.read_and_shift (data, from_css));
      m_value.push_back (single_value);
    }
  return true;
}

bool svg_base_attribute_length_list::write (QString &data, bool from_css) const
{
  for (int i = 0; i < m_value.size (); i++)
    {
      data += m_value[i].write (from_css);
      if (i != m_value.size () - 1)
        data += ", ";
    }
  return true;
}