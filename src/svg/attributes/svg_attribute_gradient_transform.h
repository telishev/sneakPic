#ifndef SVG_ATTRIBUTE_GRADIENT_TRANSFORM_H
#define SVG_ATTRIBUTE_GRADIENT_TRANSFORM_H

#include "svg/attributes/abstract_attribute.h"

#include "svg/data_types/svg_transform_list.h"

class svg_attribute_gradient_transform : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_transform_list m_transform;
public:
  svg_attribute_gradient_transform (abstract_svg_item *item) : abstract_attribute (item) {}
  virtual ~svg_attribute_gradient_transform () {}

  virtual bool read (const QString &data, bool /*from_css*/ = false) override { return m_transform.read (data); }
  virtual bool write (QString &data, bool /*to_css*/ = false) const override { return m_transform.write (data); }

  QTransform transform () const { m_transform.transform (); }
};

#endif // SVG_ATTRIBUTE_GRADIENT_TRANSFORM_H