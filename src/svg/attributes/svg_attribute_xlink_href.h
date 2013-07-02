#ifndef SVG_ATTRIBUTE_XLINK_HREF_H
#define SVG_ATTRIBUTE_XLINK_HREF_H

#include "svg/attributes/abstract_attribute.h"
#include <QString>


class svg_attribute_xlink_href : public abstract_attribute
{
  SVG_ATTRIBUTE

  QString m_element_id;
public:
  svg_attribute_xlink_href (abstract_svg_item *item);
  virtual ~svg_attribute_xlink_href ();

  virtual bool read (const QString &data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  abstract_svg_item *href () const;
};

#endif // SVG_ATTRIBUTE_XLINK_HREF_H
