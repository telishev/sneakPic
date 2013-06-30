#ifndef ABSTRACT_ATTRIBUTE_H
#define ABSTRACT_ATTRIBUTE_H

#include <QString>

#include "svg/svg_namespaces.h"
#include "svg/attributes/attribute_type.h"

#define SVG_ATTRIBUTE                                           \
public:                                                         \
  static const char *static_name ();                            \
  static svg_namespaces_t static_ns_type ();                    \
  virtual svg_attribute_type type () const override;            \
  static const char *static_ns_URI ();                          \
  virtual svg_namespaces_t namespace_type () const override;    \
  virtual const char *namespace_uri () const override;          \
  virtual const char *namespace_name () const override;         \
  virtual const char *name () const override;                   \
  virtual bool is_styleable () const override;                  \
  static bool static_is_styleable ();                           \
  static const abstract_attribute *default_value ();            \
private:                                                        \

class svg_document;
class abstract_svg_item;

class abstract_attribute
{
  abstract_svg_item *m_item;
public:
  abstract_attribute (abstract_svg_item *item) { m_item = item; }
  virtual ~abstract_attribute () {}

  virtual bool read (const QString &data, bool from_css = false) = 0;
  virtual bool write (QString &data, bool to_css = false) const = 0;

  virtual const char *name () const = 0;
  virtual const char *namespace_uri () const = 0;
  virtual const char *namespace_name () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;

  virtual svg_attribute_type type () const = 0;
  virtual bool is_styleable () const = 0;

  abstract_svg_item *item () const { return m_item; }
};

#endif // ABSTRACT_ATTRIBUTE_H
