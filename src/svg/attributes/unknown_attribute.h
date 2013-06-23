#include "abstract_attribute.h"
#ifndef UNKNOWN_ATTRIBUTE_H
#define UNKNOWN_ATTRIBUTE_H

#include "svg/attributes/abstract_attribute.h"

#include <QString>

class unknown_attribute : public abstract_attribute
{
  QString m_name;
  QString m_namespace_uri;
  QString m_namespace_name;
  QString m_value;
public:
  unknown_attribute (const QString &local_name, const QString &namespace_uri, const QString &namespace_name)
  {
    m_name = local_name;
    m_namespace_uri = namespace_uri;
    m_namespace_name = namespace_name;
  }

  ~unknown_attribute () {};

  virtual void read (const QDomAttr &item) override;
  virtual void write (QDomAttr &item) const override;

  virtual QString name () const override { return m_name;}
  virtual QString namespace_uri () const override { return m_namespace_uri; }

  virtual attribute_type type () const override { return attribute_type::UNKNOWN; }

  virtual QString namespace_name () const override { return m_namespace_name; }
  virtual svg_namespaces_t namespace_type () const override { return svg_namespaces_t::UNKNOWN; }
};

#endif // UNKNOWN_ATTRIBUTE_H
