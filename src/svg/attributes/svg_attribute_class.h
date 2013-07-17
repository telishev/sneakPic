#ifndef SVG_ATTRIBUTE_CLASS_H
#define SVG_ATTRIBUTE_CLASS_H

#include "svg/attributes/abstract_attribute.h"

#include <QStringList>

class svg_attribute_class : public abstract_attribute
{
  SVG_ATTRIBUTE
  QStringList m_class_names;
public:
  svg_attribute_class (abstract_svg_item *item);
  virtual ~svg_attribute_class ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  bool is_class (const QString &class_name) const;
};


#endif // SVG_ATTRIBUTE_CLASS_H
