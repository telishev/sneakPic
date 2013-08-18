#ifndef SVG_ATTRIBUTE_GRADIENT_TRANSFORM_H
#define SVG_ATTRIBUTE_GRADIENT_TRANSFORM_H

#include "svg/attributes/abstract_attribute.h"

#include "svg/data_types/svg_transform_list.h"

class svg_attribute_gradient_transform : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_transform_list m_transform;
public:
  svg_attribute_gradient_transform (svg_document *document) : abstract_attribute (document) {}
  virtual ~svg_attribute_gradient_transform () {}

  virtual bool read (const char *data, bool /*from_css*/ = false) override { return m_transform.read (data); }
  virtual bool write (QString &data, bool /*to_css*/ = false) const override { return m_transform.write (data); }

  QTransform transform () const { return m_transform.transform (); }
};

#endif // SVG_ATTRIBUTE_GRADIENT_TRANSFORM_H
