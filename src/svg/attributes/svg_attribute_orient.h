#ifndef SVG_ATTRIBUTE_ORIENT_H
#define SVG_ATTRIBUTE_ORIENT_H

#include "svg/data_types/svg_data_type_angle.h"
#include "svg/attributes/abstract_attribute.h"

#include <QColor>

class svg_attribute_orient : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_data_type_angle m_angle;
  bool m_auto;
public:
  svg_attribute_orient (abstract_svg_item *item);
  virtual ~svg_attribute_orient ();

  double value (QPointF bisector) const;

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};
#endif // SVG_ATTRIBUTE_STROKE_WIDTH_H


