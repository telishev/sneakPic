#ifndef SVG_ATTRIBUTES_LENGTH_TYPE_H
#define SVG_ATTRIBUTES_LENGTH_TYPE_H

#include "svg/attributes/svg_base_attribute_length.h"

class svg_attribute_width : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_width (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::X) {}
};

class svg_attribute_height : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:

  svg_attribute_height (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {}
};

class svg_attribute_x1 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x1 (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::X) {}
};

class svg_attribute_x2 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x2 (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::X) {}
};

class svg_attribute_y1 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y1 (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {}
};

class svg_attribute_y2 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y2 (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {}
};

class svg_attribute_x : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS) 
    : svg_base_attribute_length  (value, units, units_orientation::X) {}
};

class svg_attribute_y : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {}
};

class svg_attribute_r : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_r (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units) {}
};

class svg_attribute_rx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_rx (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_ry : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_ry (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_cx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_cx (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::X) {}
};

class svg_attribute_cy : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_cy (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {}
};

class svg_attribute_fx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_fx (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_fy : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_fy (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_refx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_refx (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_refy : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_refy (double value = 0.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length  (value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_marker_width : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_marker_width(double value = 3.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (value, units, units_orientation::X) {} // TODO: Checkout is units orientation really applied here
};

class svg_attribute_marker_height : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_marker_height(double value = 3.0, svg_length_units units = svg_length_units::NO_UNITS)
    : svg_base_attribute_length (value, units, units_orientation::Y) {} // TODO: Checkout is units orientation really applied here
};


#endif // SVG_ATTRIBUTES_LENGTH_TYPE_H
