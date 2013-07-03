#ifndef SVG_ATTRIBUTES_ENUM_H
#define SVG_ATTRIBUTES_ENUM_H

#include "svg/attributes/abstract_attribute.h"

#include "common/enum_conversion.h"

#include <QString>

template<typename T>
class svg_base_attribute_enum : public abstract_attribute
{
protected:
  T m_value;
public:
  svg_base_attribute_enum (abstract_svg_item *item) : abstract_attribute (item) { m_value = T (); }
  virtual ~svg_base_attribute_enum () {}

  virtual bool read (const QString &data, bool /*from_css*/ = false) override
  {
    m_value = string_to_enum<T> (data.toUtf8 ().constData ());
    return m_value != (T)enum_values_count (T ());
  }

  virtual bool write (QString &data, bool /*to_css*/ = false) const override
  {
    data = enum_to_string (m_value);
    return true;
  }

  T value () const { return m_value; }
};

enum class gradient_units
{
  USER_SPACE, 
  OBJECT_BOUNDING_BOX,

  INVALID,
};

const char *enum_to_string (gradient_units id);
static inline int enum_values_count (gradient_units) { return (int)gradient_units::INVALID; }

class svg_attribute_gradient_units : public svg_base_attribute_enum<gradient_units>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_gradient_units (abstract_svg_item *item) : svg_base_attribute_enum (item)
  { m_value = gradient_units::OBJECT_BOUNDING_BOX; }
};

enum class spread_method
{
  PAD,
  REFLECT,
  REPEAT,

  INVALID,
};

const char *enum_to_string (spread_method id);
static inline int enum_values_count (spread_method) { return (int)spread_method::INVALID; }

class svg_attribute_spread_method : public svg_base_attribute_enum<spread_method>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_spread_method (abstract_svg_item *item) : svg_base_attribute_enum (item)
  { m_value = spread_method::PAD; }
};


#endif // SVG_ATTRIBUTES_ENUM_H
