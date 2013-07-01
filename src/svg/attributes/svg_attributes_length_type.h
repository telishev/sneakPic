#ifndef SVG_ATTRIBUTES_LENGTH_TYPE_H
#define SVG_ATTRIBUTES_LENGTH_TYPE_H

#include "svg/attributes/svg_base_attribute_length.h"

class svg_attribute_width : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_width (abstract_svg_item *item) : svg_base_attribute_length (item) {}

  svg_attribute_width (double value, svg_length_units units)
    : svg_base_attribute_length (0)
  {
    m_value.set_value (value, units); /// A4
  }
};

class svg_attribute_height : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:

  svg_attribute_height (abstract_svg_item *item) : svg_base_attribute_length (item) {}

  svg_attribute_height (double value, svg_length_units units)
    : svg_base_attribute_length (0)
  {
    m_value.set_value (value, units); /// A4
  }
};

class svg_attribute_x1 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x1 (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_x2 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x2 (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_y1 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y1 (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_y2 : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y2 (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_x : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_x (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_y : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_y (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_r : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_r (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_rx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_rx (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_ry : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_ry (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_cx : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_cx (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};

class svg_attribute_cy : public svg_base_attribute_length
{
  SVG_ATTRIBUTE

public:
  svg_attribute_cy (abstract_svg_item *item) : svg_base_attribute_length (item) {}
};



#endif // SVG_ATTRIBUTES_LENGTH_TYPE_H
