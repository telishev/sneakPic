#ifndef UNKNOWN_item_H
#define UNKNOWN_item_H

#include "svg/items/abstract_svg_item.h"

class unknown_item : public abstract_svg_item
{
  std::string m_name;
  std::string m_namespace_uri;
  std::string m_namespace_name;
public:
  unknown_item (svg_document *document, const QString &name, const QString &namespace_uri, const QString &prefix)
    : abstract_svg_item (document)
  {
    m_name = name.toStdString ();
    m_namespace_uri = namespace_uri.toStdString ();
    m_namespace_name = prefix.toStdString ();
  }

  virtual ~unknown_item () override {}
  virtual svg_item_type type () const override { return svg_item_type::UNKNOWN; }

  virtual const char *name () const override { return m_name.c_str (); }
  virtual const char *namespace_uri () const override { return m_namespace_uri.c_str (); }
  virtual const char *namespace_name () const override { return m_namespace_name.c_str (); }
  virtual svg_namespaces_t namespace_type () const override { return svg_namespaces_t::UNKNOWN; }
};


#endif // UNKNOWN_item_H

