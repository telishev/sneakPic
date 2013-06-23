#ifndef UNKNOWN_item_H
#define UNKNOWN_item_H

#include "svg/items/abstract_svg_item.h"

class unknown_item : public abstract_svg_item
{
  QString m_name;
  QString m_namespace_uri;
  QString m_namespace_name;
public:
  unknown_item (svg_document *document, const QString &name, const QString &namespace_uri, const QString &prefix)
    : abstract_svg_item (document) { m_name = name; m_namespace_uri = namespace_uri; m_namespace_name = prefix; }
  virtual ~unknown_item () override {}
  virtual svg_item_type type () const override { return svg_item_type::UNKNOWN; }

  virtual QString name () const override { return m_name; }
  virtual QString namespace_uri () const override { return m_namespace_uri; }
  virtual QString namespace_name () const override { return m_namespace_name; }
  virtual svg_namespaces_t namespace_type () const override { return svg_namespaces_t::UNKNOWN; }
};


#endif // UNKNOWN_item_H

