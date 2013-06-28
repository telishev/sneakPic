#ifndef ABSTRACT_ATTRIBUTE_H
#define ABSTRACT_ATTRIBUTE_H

#include <QString>

#include "svg/svg_namespaces.h"
#include "svg/attributes/attribute_type.h"

#define SVG_ATTRIBUTE                                           \
public:                                                         \
  static QString static_name ();                                \
  static svg_namespaces_t static_ns_type ();                    \
  virtual svg_attribute_type type () const override;            \
  static QString static_ns_URI ();                              \
  virtual svg_namespaces_t namespace_type () const override;    \
  virtual QString namespace_uri () const override;              \
  virtual QString namespace_name () const override;             \
  virtual QString name () const override;                       \
private:                                                        \

class abstract_attribute
{
public:
  virtual ~abstract_attribute () {}

  virtual bool read (const QString &data) = 0;
  virtual bool write (QString &data) const = 0;

  virtual QString name () const = 0;
  virtual QString namespace_uri () const = 0;
  virtual QString namespace_name () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;

  virtual svg_attribute_type type () const = 0;
};

#endif // ABSTRACT_ATTRIBUTE_H
