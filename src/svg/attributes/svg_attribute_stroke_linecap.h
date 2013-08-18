#ifndef SVG_ATTRIBUTE_STROKE_LINECAP_H
#define SVG_ATTRIBUTE_STROKE_LINECAP_H

#include "svg/attributes/abstract_attribute.h"

#include <QString>

class svg_attribute_stroke_linecap : public abstract_attribute
{
  SVG_ATTRIBUTE

  Qt::PenCapStyle m_linecap;
public:
  svg_attribute_stroke_linecap (svg_document *document);
  virtual ~svg_attribute_stroke_linecap ();

  Qt::PenCapStyle get_stroke_linecap () const { return m_linecap; }

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};
#endif // SVG_ATTRIBUTE_STROKE_LINECAP_H
