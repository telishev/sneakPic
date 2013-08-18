#ifndef SVG_ATTRIBUTE_STYLE_H
#define SVG_ATTRIBUTE_STYLE_H

#include "svg/attributes/abstract_attribute.h"

#include <string>

class css_declaration;

class svg_attribute_style : public abstract_attribute
{
  SVG_ATTRIBUTE

  css_declaration *declaration;
public:
  svg_attribute_style (svg_document *document);
  virtual ~svg_attribute_style ();

  abstract_attribute *get_attribute (const std::string &str) const;

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};

#endif // SVG_ATTRIBUTE_STYLE_H
