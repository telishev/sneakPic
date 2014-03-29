#ifndef SVG_ATTRIBUTE_OFFSET_H
#define SVG_ATTRIBUTE_OFFSET_H

#include "svg/attributes/abstract_attribute.h"

class svg_attribute_offset : public abstract_attribute
{
  SVG_ATTRIBUTE

  double m_offset;
public:
  svg_attribute_offset ()  { m_offset = 0.0; }
  virtual ~svg_attribute_offset () {}

  virtual bool read (const char *data, bool /*from_css*/ = false) override;
  virtual bool write (QString &data, bool /*to_css*/ = false) const override;

  double offset () const { return m_offset; }
  void set_offset (double value) { m_offset = value; }

};

#endif // SVG_ATTRIBUTE_OFFSET_H
