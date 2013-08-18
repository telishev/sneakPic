#ifndef SVG_ATTRIBUTES_NUMBER_H
#define SVG_ATTRIBUTES_NUMBER_H

#include "svg/attributes/abstract_attribute.h"

class svg_base_attribute_number : public abstract_attribute
{
protected:
  double m_value;
public:
  svg_base_attribute_number (svg_document *document) : abstract_attribute (document) { m_value = 0.0; }

  double value () const { return m_value; }

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool from_css = false) const override;
};

class svg_attribute_opacity : public svg_base_attribute_number
{
  SVG_ATTRIBUTE
public:
  svg_attribute_opacity (svg_document *document) : svg_base_attribute_number (document) { m_value = 1.0; }

  double computed_opacity () const;
};

class svg_attribute_stroke_opacity : public svg_base_attribute_number
{
  SVG_ATTRIBUTE
public:
  svg_attribute_stroke_opacity (svg_document *document) : svg_base_attribute_number (document) { m_value = 1.0; }
};

class svg_attribute_fill_opacity : public svg_base_attribute_number
{
  SVG_ATTRIBUTE
public:
  svg_attribute_fill_opacity (svg_document *document) : svg_base_attribute_number (document) { m_value = 1.0; }
};

class svg_attribute_stop_opacity : public svg_base_attribute_number
{
  SVG_ATTRIBUTE
public:
  svg_attribute_stop_opacity (svg_document *document) : svg_base_attribute_number (document) { m_value = 1.0; }
};

class svg_attribute_stroke_dash_offset : public svg_base_attribute_number
{
  SVG_ATTRIBUTE
public:
  svg_attribute_stroke_dash_offset (svg_document *document) : svg_base_attribute_number (document) { m_value = 0.0; }
};

#endif // SVG_ATTRIBUTES_NUMBER_H
