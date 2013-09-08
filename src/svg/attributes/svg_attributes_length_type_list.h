#ifndef SVG_ATTRIBUTES_LENGTH_TYPE_LIST_H
#define SVG_ATTRIBUTES_LENGTH_TYPE_LIST_H

#include "svg/attributes/svg_base_attribute_length_list.h"
#include "svg/data_types/svg_data_type_length.h"

#include <QList>
class svg_attribute_dx : public svg_base_attribute_length_list
{
  SVG_ATTRIBUTE

public:
  svg_attribute_dx () : svg_base_attribute_length_list (units_orientation::X) {}
};

class svg_attribute_dy : public svg_base_attribute_length_list
{
  SVG_ATTRIBUTE

public:
  svg_attribute_dy () : svg_base_attribute_length_list (units_orientation::Y) {}
};

class svg_attribute_stroke_dash_array : public svg_base_attribute_length_list
{
  SVG_ATTRIBUTE

public:
  svg_attribute_stroke_dash_array () : svg_base_attribute_length_list (units_orientation::OTHER) {}

  virtual bool read (const char *data, bool from_css = false) override
  { 
     if (strcmp (data, "none") != 0)
       {
        bool res = svg_base_attribute_length_list::read (data, from_css);
        if (res && m_value.size () % 2 == 1)
          m_value.append (m_value);
        return res;
       }
     return true;
  };

  virtual bool write (QString &data, bool from_css = false) const override
  {
    if (m_value.size () == 0)
      data = "none";
    else
      svg_base_attribute_length_list::write (data, from_css);
    return true;
  }
};

#endif // SVG_ATTRIBUTES_LENGTH_TYPE_LIST_H
