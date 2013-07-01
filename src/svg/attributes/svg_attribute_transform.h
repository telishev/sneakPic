#ifndef SVG_ATTRIBUTE_TRANSFORM_H
#define SVG_ATTRIBUTE_TRANSFORM_H

#include "svg/attributes/abstract_attribute.h"

#include <QTransform>
#include <vector>
#include <string>

class svg_attribute_transform : public abstract_attribute
{
  SVG_ATTRIBUTE

  QTransform m_transform;
public:
  svg_attribute_transform (abstract_svg_item *item);
  virtual ~svg_attribute_transform ();

  virtual bool read (const QString &data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  QTransform computed_transform () const;

private:
  bool read (const char *data);
  bool get_transfrom_data (const char *data, std::vector<double> &dest) const;
  bool apply_transform (const std::string &name, std::vector<double> &data);
};


#endif // SVG_ATTRIBUTE_TRANSFORM_H
