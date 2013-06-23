#ifndef SVG_ATTRIBUTE_HEIGHT_H
#define SVG_ATTRIBUTE_HEIGHT_H

#include "svg/attributes/abstract_attribute.h"

class svg_attribute_height : public abstract_attribute
{
  SVG_ATTRIBUTE ("height", svg_attribute_type::HEIGHT, svg_namespaces_t::EMPTY)

  double height;
public:
  svg_attribute_height () { height = 0; }
  virtual ~svg_attribute_height () {}

  virtual bool read (const QString &data) override { bool ok; height = data.toDouble (&ok); return ok; }
  virtual bool write (QString &data) const override { data = QString::number (height) ; return true; }
};

#endif // SVG_ATTRIBUTE_HEIGHT_H
