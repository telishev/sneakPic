#ifndef SVG_ATTRIBUTE_TRANSFORM_H
#define SVG_ATTRIBUTE_TRANSFORM_H

#include "svg/attributes/abstract_attribute.h"

#include "svg/data_types/svg_transform_list.h"

class svg_attribute_transform : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_transform_list m_transform;
  /// TODO: remove this hack for "use" item
  QTransform m_additional_transform;
public:
  svg_attribute_transform (svg_document *document);
  virtual ~svg_attribute_transform ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  QTransform computed_transform () const;
  void set_additional_transform (const QTransform &additional_transform);

  void set_transform (const QTransform &transform);
};


#endif // SVG_ATTRIBUTE_TRANSFORM_H
