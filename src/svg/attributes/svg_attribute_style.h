#ifndef SVG_ATTRIBUTE_STYLE_H
#define SVG_ATTRIBUTE_STYLE_H

#include "svg/attributes/abstract_attribute.h"

class svg_attribute_style : public abstract_attribute
{
  SVG_ATTRIBUTE ("style", svg_attribute_type::STYLE, svg_namespaces_t::EMPTY)

public:
  svg_attribute_style ();
  virtual ~svg_attribute_style ();

  virtual bool read (const QString &data) override;
  virtual bool write (QString &data) const override;
};

#endif // SVG_ATTRIBUTE_STYLE_H
