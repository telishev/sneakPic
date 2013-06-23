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
  svg_attribute_id ();
  virtual ~svg_attribute_id ();

  virtual void read (const QDomAttr &item) override;
  virtual void write (QDomAttr &item) const override;

  static QString static_name () { return "id"; }
  static svg_namespaces_t static_ns_type () { return svg_namespaces_t::EMPTY; }

  virtual attribute_type type () const override { return attribute_type::ID; }

  QString id () const { return m_id; }
  void set_id (const QString &id) { m_id = id; } 
};

#endif // SVG_ATTRIBUTE_ID_H
