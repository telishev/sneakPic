#ifndef SVG_ATTRIBUTE_HEIGHT_H
#define SVG_ATTRIBUTE_HEIGHT_H

#include "svg/attributes/abstract_attribute.h"

class svg_attribute_height : public abstract_attribute
{
  SVG_ATTRIBUTE

  double m_height;
public:
  svg_attribute_height (abstract_svg_item *item);
  virtual ~svg_attribute_height ();

  double height () const { return m_height; }

  virtual bool read (const QString &data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override; 
};

#endif // SVG_ATTRIBUTE_HEIGHT_H
