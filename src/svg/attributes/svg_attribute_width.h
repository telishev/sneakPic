#ifndef SVG_ATTRIBUTE_WIDTH_H
#define SVG_ATTRIBUTE_WIDTH_H

#include "svg/attributes/abstract_attribute.h"

class svg_attribute_width : public abstract_attribute
{
  SVG_ATTRIBUTE ("width", svg_attribute_type::WIDTH, svg_namespaces_t::EMPTY)

  double width;
public:
  svg_attribute_width () { width = 0; }
  virtual ~svg_attribute_width () {}

  virtual bool read (const QString &data) override { bool ok; width = data.toDouble (&ok); return ok; }
  virtual bool write (QString &data) const override { data = QString::number (width) ; return true; }
};


#endif // SVG_ATTRIBUTE_WIDTH_H
