#ifndef SVG_ATTRIBUTE_XLINK_HREF_H
#define SVG_ATTRIBUTE_XLINK_HREF_H

#include "svg_iri_attributes.h"


class svg_attribute_xlink_href : public svg_iri_attribute
{
  SVG_ATTRIBUTE

public:
  svg_attribute_xlink_href ();
  virtual ~svg_attribute_xlink_href ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  string get_fragment_name () const;
  QImage get_image_data (const QString &svg_name) const;
  iri_type get_data_type () const;

};

#endif // SVG_ATTRIBUTE_XLINK_HREF_H
