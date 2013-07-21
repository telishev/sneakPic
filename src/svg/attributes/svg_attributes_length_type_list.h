#ifndef SVG_ATTRIBUTES_LENGTH_TYPE_LIST_H
#define SVG_ATTRIBUTES_LENGTH_TYPE_LIST_H

#include "svg/attributes/svg_base_attribute_length_list.h"
#include "svg/data_types/svg_data_type_length.h"

#include <QList>
class svg_attribute_dx : public svg_base_attribute_length_list
{
  SVG_ATTRIBUTE

public:
  svg_attribute_dx(abstract_svg_item *item)
    : svg_base_attribute_length_list (item, units_orientation::X) {}
};

class svg_attribute_dy : public svg_base_attribute_length_list
{
  SVG_ATTRIBUTE

public:
  svg_attribute_dy(abstract_svg_item *item)
    : svg_base_attribute_length_list (item, units_orientation::Y) {}
};


#endif // SVG_ATTRIBUTES_LENGTH_TYPE_LIST_H
