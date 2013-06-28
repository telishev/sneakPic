#ifndef SVG_ATTRIBUTE_WIDTH_H
#define SVG_ATTRIBUTE_WIDTH_H

#include "svg/attributes/abstract_attribute.h"

class svg_attribute_width : public abstract_attribute
{
  SVG_ATTRIBUTE

  double m_width;
public:
  svg_attribute_width () { m_width = 0; }
  virtual ~svg_attribute_width () {}

  double width () const { return m_width; }

  virtual bool read (const QString &data) override { bool ok; m_width = data.toDouble (&ok); return ok; }
  virtual bool write (QString &data) const override { data = QString::number (m_width) ; return true; }
};


#endif // SVG_ATTRIBUTE_WIDTH_H
