#ifndef SVG_BASE_ATTRIBUTE_LENGTH_H
#define SVG_BASE_ATTRIBUTE_LENGTH_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_length.h"

class svg_base_attribute_length : public abstract_attribute
{
protected:
  svg_data_type_length m_value;
public:
  svg_base_attribute_length (abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : abstract_attribute (item) { m_value.set_value (value, units); }

  double value () const { return m_value.value (); }

  virtual bool read (const char *data, bool from_css = false) override
  {
    return m_value.read (data, from_css);
  }
  virtual bool write (QString &data, bool from_css = false) const override
  {
    data = m_value.write (from_css);
    return true;
  }
};


#endif // SVG_BASE_ATTRIBUTE_LENGTH_H
