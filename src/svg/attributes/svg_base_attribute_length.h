#ifndef SVG_BASE_ATTRIBUTE_LENGTH_H
#define SVG_BASE_ATTRIBUTE_LENGTH_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_length.h"

class svg_base_attribute_length : public abstract_attribute
{
protected:
  svg_data_type_length m_value;
public:
  svg_base_attribute_length (abstract_svg_item *item) : abstract_attribute (item) {}

  double value () const { return m_value.value (); }

  virtual bool read (const QString &data, bool from_css = false) override
  {
    return m_value.read (data.toUtf8 ().constData (), from_css);
  }
  virtual bool write (QString &data, bool from_css = false) const override
  {
    data = m_value.write (from_css);
    return true;
  }
};


#endif // SVG_BASE_ATTRIBUTE_LENGTH_H
