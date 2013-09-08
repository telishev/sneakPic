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
  svg_base_attribute_enum () { m_value = T (); }
  virtual ~svg_base_attribute_enum () {}

  virtual bool read (const char *data, bool /*from_css*/ = false) override
  {
    m_value = string_to_enum<T> (data);
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
  svg_attribute_gradient_units ()
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
  svg_attribute_spread_method ()
  { m_value = spread_method::PAD; }
};

enum class fill_rule
{
  NON_ZERO,
  EVEN_ODD,
  INVALID,
};

const char *enum_to_string (fill_rule id);
static inline int enum_values_count (fill_rule) { return (int)fill_rule::INVALID; }

class svg_attribute_fill_rule : public svg_base_attribute_enum<fill_rule>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_fill_rule ()
  { m_value = fill_rule::NON_ZERO; }
};


class svg_attribute_clip_rule : public svg_base_attribute_enum<fill_rule>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_clip_rule ()
  { m_value = fill_rule::NON_ZERO; }
};

enum class visibility
{
  VISIBLE,
  HIDDEN,
  COLLAPSE,

  INVALID
};

const char *enum_to_string (visibility id);
static inline int enum_values_count (visibility) { return (int)visibility::INVALID; }

class svg_attribute_visibility : public svg_base_attribute_enum<visibility>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_visibility ()
  { m_value = visibility::VISIBLE; }
};

enum class display // Seriously like that many?
{
  INLINE,
  BLOCK,
  LIST_ITEM,
  RUN_IN,
  COMPACT,
  MARKER,
  TABLE,
  INLINE_TABLE,
  TABLE_ROW_GROUP,
  TABLE_HEADER_GROUP,
  TABLE_FOOTER_GROUP,
  TABLE_ROW,
  TABLE_COLUMN_GROUP,
  TABLE_COLUMN,
  TABLE_CELL,
  TABLE_CAPTION,
  NONE,

  INVALID
};

const char *enum_to_string (display id);
static inline int enum_values_count (display) { return (int)display::INVALID; }

class svg_attribute_display : public svg_base_attribute_enum<display>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_display ()
  { m_value = display::INLINE; }
};

enum class text_anchor
{
  START,
  MIDDLE,
  END,

  INVALID
};

const char *enum_to_string (text_anchor id);
static inline int enum_values_count (text_anchor) { return (int)text_anchor::INVALID; }

class svg_attribute_text_anchor : public svg_base_attribute_enum<text_anchor>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_text_anchor ()
  { m_value = text_anchor::START; }
};

enum class marker_units
{
  STROKE_WIDTH, 
  USER_SPACE,

  INVALID,
};

const char *enum_to_string (marker_units id);
static inline int enum_values_count (marker_units) { return (int)marker_units::INVALID; }

class svg_attribute_marker_units : public svg_base_attribute_enum<marker_units>
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker_units ()
  { m_value = marker_units::STROKE_WIDTH; }
};

#endif // SVG_ATTRIBUTES_ENUM_H
