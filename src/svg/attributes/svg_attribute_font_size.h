#ifndef SVG_ATTRIBUTE_FONT_SIZE_H
#define SVG_ATTRIBUTE_FONT_SIZE_H

#include "svg/attributes/abstract_attribute.h"

#include "svg/data_types/svg_data_type_length.h"

class svg_attribute_font_size : public abstract_attribute
{
  SVG_ATTRIBUTE
private:
  svg_data_type_length m_font_size;
public:
  svg_attribute_font_size (abstract_svg_item *item);
  virtual ~svg_attribute_font_size ();
  
  double value () const {return m_font_size.value (item (), units_orientation::OTHER); }

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};
#endif // SVG_ATTRIBUTE_FONT_SIZE_H
