#ifndef SVG_ATTRIBUTE_STROKE_WIDTH_H
#define SVG_ATTRIBUTE_STROKE_WIDTH_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_length.h"

#include <QColor>

class svg_attribute_stroke_width : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_data_type_length m_stroke_width;
public:
  svg_attribute_stroke_width (abstract_svg_item *item);
  virtual ~svg_attribute_stroke_width ();

  double get_stroke_width () const { return m_stroke_width.value (item (), units_orientation::OTHER); }

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};
#endif // SVG_ATTRIBUTE_STROKE_WIDTH_H


