#ifndef SVG_ATTRIBUTE_WIDTH_H
#define SVG_ATTRIBUTE_WIDTH_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_length.h"

class svg_attribute_width : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_data_type_length m_width;
public:
  svg_attribute_width (abstract_svg_item *item);
  virtual ~svg_attribute_width ();

  double width () const;

  virtual bool read (const QString &data, bool from_css = false) override;
  virtual bool write (QString &data, bool from_css = false) const override;
};


#endif // SVG_ATTRIBUTE_WIDTH_H
