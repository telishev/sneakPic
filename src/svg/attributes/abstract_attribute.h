#ifndef ABSTRACT_ATTRIBUTE_H
#define ABSTRACT_ATTRIBUTE_H

class QDomAttr;

#include <QString>

#include "svg/svg_namespaces.h"

enum class attribute_type
{
  UNKNOWN,
  ID,
};

#define SVG_ATTRIBUTE                                                                                         \
public:                                                                                                       \
  static QString static_ns_URI () { return svg_namespaces::uri (static_ns_type ()); }                         \
  virtual svg_namespaces_t namespace_type () const override { return static_ns_type (); }                     \
  virtual QString namespace_uri () const override { return svg_namespaces::uri (namespace_type ()); }         \
  virtual QString namespace_name () const override { return svg_namespaces::name (namespace_type ()); }       \
  virtual QString name () const override { return static_name (); }                                           \
private:                                                                                                      \

class abstract_attribute
{
public:
  virtual ~abstract_attribute () {}

  virtual void read (const QDomAttr &item) = 0;
  virtual void write (QDomAttr &item) const = 0;

  virtual QString name () const = 0;
  virtual QString namespace_uri () const = 0;
  virtual QString namespace_name () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;

  virtual attribute_type type () const = 0;
};

#endif // ABSTRACT_ATTRIBUTE_H
