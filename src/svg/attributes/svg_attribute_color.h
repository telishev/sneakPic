#ifndef SVG_ATTRIBUTE_COLOR_H
#define SVG_ATTRIBUTE_COLOR_H

#include "svg/attributes/abstract_attribute.h"

#include <QColor>

class css_declaration;

class svg_attribute_color : public abstract_attribute
{
  SVG_ATTRIBUTE

  css_declaration *declaration;
  QColor m_color;
public:
  svg_attribute_color (svg_document *document);
  virtual ~svg_attribute_color ();

  QColor value () const { return m_color; }

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};

#endif // SVG_ATTRIBUTE_COLOR_H
