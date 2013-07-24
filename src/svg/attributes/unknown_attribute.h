#ifndef UNKNOWN_ATTRIBUTE_H
#define UNKNOWN_ATTRIBUTE_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/svg_namespaces.h"
#include "svg/attributes/svg_attribute_type.h"

class unknown_attribute : public abstract_attribute
{
  std::string m_name;
  std::string m_namespace_uri;
  std::string m_namespace_name;
  QString m_value;
public:
  unknown_attribute (abstract_svg_item *item, const char *local_name, const char *namespace_uri, const char *namespace_name)
    : abstract_attribute (item)
  {
    m_name = local_name;
    m_namespace_uri = namespace_uri;
    m_namespace_name = namespace_name;
  }

  ~unknown_attribute () {};

  virtual bool read (const char *data, bool /*from_css*/ = false) override { m_value = data; return true; }
  virtual bool write (QString &data, bool /*to_css*/ = false) const override { data = m_value; return true; }

  virtual const char *type_name () const override { return m_name.c_str ();}
  virtual const char *namespace_uri () const override { return m_namespace_uri.c_str (); }

  virtual svg_attribute_type type () const override { return svg_attribute_type::UNKNOWN; }

  virtual const char *namespace_name () const override { return m_namespace_name.c_str (); }
  virtual svg_namespaces_t namespace_type () const override { return svg_namespaces_t::UNKNOWN; }

  virtual svg_inherit_type inherit_type () const override { return svg_inherit_type::NONE; }

};

#endif // UNKNOWN_ATTRIBUTE_H
