#include "svg/attributes/svg_attribute_class.h"




svg_attribute_class::svg_attribute_class ()
{

}

svg_attribute_class::~svg_attribute_class ()
{

}

bool svg_attribute_class::read (const QString &data)
{
  m_class_names = data.split ("\0x20""\0x9""0xD""0xA");
  return m_class_names.size () != 0;
}

bool svg_attribute_class::write (QString &data) const 
{
  if (m_class_names.size () == 0)
    return false;

  data = m_class_names[0];
  for (int i = 1; i < m_class_names.size (); i++)
    {
      data += " ";
      data += m_class_names[1];
    }

  return true;
}

bool svg_attribute_class::is_class (const QString &class_name) const
{
  return m_class_names.contains (class_name);
}
