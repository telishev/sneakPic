#ifndef SVG_ATTRIBUTE_ID_H
#define SVG_ATTRIBUTE_ID_H

#include "svg/attributes/abstract_attribute.h"

#include <QString>
#include "svg/svg_namespaces.h"

class svg_attribute_id : public abstract_attribute
{
  SVG_ATTRIBUTE
  QString m_id;
public:
  svg_attribute_id (abstract_svg_item *item) : abstract_attribute (item) {}
  virtual ~svg_attribute_id () {}

  virtual bool read (const QString &data, bool /*from_css*/ = false) override { m_id = data; return true; }
  virtual bool write (QString &data, bool /*to_css*/ = false) const override { data = m_id; return true; }

  QString id () const { return m_id; }
  void set_id (const QString &id) { m_id = id; } 
};

#endif // SVG_ATTRIBUTE_ID_H
