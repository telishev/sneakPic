#ifndef SVG_ATTRIBUTE_CLASS_H
#define SVG_ATTRIBUTE_CLASS_H

#include "svg/attributes/abstract_attribute.h"

#include <QStringList>
#include "svg/svg_namespaces.h"

class svg_attribute_class : public abstract_attribute
{
  SVG_ATTRIBUTE
  QStringList m_class_names;
public:
  svg_attribute_class ();
  virtual ~svg_attribute_class ();

  virtual bool read (const QString &data) override;
  virtual bool write (QString &data) const override;

  bool is_class (const QString &class_name) const;
};


#endif // SVG_ATTRIBUTE_CLASS_H
