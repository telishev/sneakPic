#ifndef SVG_BASE_ATTRIBUTE_LENGTH_H
#define SVG_BASE_ATTRIBUTE_LENGTH_H

#include "common/common_utils.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_length.h"

#include <QList>

class svg_base_attribute_length : public abstract_attribute
{
protected:
  svg_data_type_length m_value;
  units_orientation m_units_orientation;
public:
  svg_base_attribute_length (svg_document *document, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS, 
                             units_orientation units_orientation_arg = units_orientation::OTHER)
    : abstract_attribute (document) { m_value.set_value (value, units); m_units_orientation = units_orientation_arg; }

  double value () const { return m_value.value (item (), m_units_orientation); }
  svg_length_units units_type () const { return m_value.units_type (); }

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
