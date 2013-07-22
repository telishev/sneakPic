#ifndef SVG_ATTRIBUTE_XLINK_HREF_H
#define SVG_ATTRIBUTE_XLINK_HREF_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_iri.h"


class svg_attribute_xlink_href : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_data_type_iri m_iri;
public:
  svg_attribute_xlink_href (abstract_svg_item *item);
  virtual ~svg_attribute_xlink_href ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  abstract_svg_item *get_fragment () const;
  bool has_image_data () const;
  QImage *get_image_data () const;
  iri_type get_data_type () const;
};

#endif // SVG_ATTRIBUTE_XLINK_HREF_H
