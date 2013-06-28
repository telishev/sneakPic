#ifndef SVG_ATTRIBUTE_VERSION_H
#define SVG_ATTRIBUTE_VERSION_H

#include "svg/attributes/abstract_attribute.h"

class svg_attribute_version : public abstract_attribute
{
  SVG_ATTRIBUTE

  QString version;
public:
  svg_attribute_version () {}
  virtual ~svg_attribute_version () {}

  virtual bool read (const QString &data) override { version = data; return true; }
  virtual bool write (QString &data) const override { data = version ; return true; }
};

#endif // SVG_ATTRIBUTE_VERSION_H
