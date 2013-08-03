#ifndef SVG_ATTRIBUTES_LENGTH_TYPE_H
#define SVG_ATTRIBUTES_LENGTH_TYPE_H

#include "svg/attributes/svg_base_attribute_length.h"

class svg_attribute_width : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_width(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {}
};

class svg_attribute_height : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:

  svg_attribute_height(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {}
};

class svg_attribute_x1 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x1(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {}
};

class svg_attribute_x2 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x2(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {}
};

class svg_attribute_y1 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y1(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {}
};

class svg_attribute_y2 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y2(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {}
};

class svg_attribute_x : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS) 
    : svg_base_attribute_length (item, value, units, units_orientation::X) {}
};

class svg_attribute_y : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {}
};

class svg_attribute_r : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_r(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units) {}
};

class svg_attribute_rx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_rx(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_ry : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_ry(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_cx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_cx(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {}
};

class svg_attribute_cy : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_cy(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {}
};

class svg_attribute_fx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_fx(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_fy : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_fy(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_refx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_refx(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_refy : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_refy(abstract_svg_item *item, double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_marker_width : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_marker_width(abstract_svg_item *item, double value = 3.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_marker_height : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_marker_height(abstract_svg_item *item, double value = 3.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (item, value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};


#endif // SVG_ATTRIBUTES_LENGTH_TYPE_H
