#ifndef SVG_BASE_ATTRIBUTE_LENGTH_LIST_H
#define SVG_BASE_ATTRIBUTE_LENGTH_LIST_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_length.h"

#include <QList>

class svg_base_attribute_length_list : public abstract_attribute
{
protected:
  QList <svg_data_type_length> m_value;
  units_orientation m_units_orientation;
public:
  svg_base_attribute_length_list (abstract_svg_item *item, units_orientation units_orientation_arg = units_orientation::OTHER)
    : abstract_attribute (item) {  m_units_orientation = units_orientation_arg; }

  void fill_values (QList<double> &list) const;

  virtual bool read (const char *data, bool from_css = false) override;

  virtual bool write (QString &data, bool from_css = false) const override;
};


#endif // SVG_BASE_ATTRIBUTE_LENGTH_LIST_H
