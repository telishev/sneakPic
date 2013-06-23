#ifndef ABSTRACT_ITEM_H
#define ABSTRACT_ITEM_H



class QDomElement;
class QDomDocument;
class svg_document;

enum class svg_namespaces_t;

#include <QString>

class abstract_item
{
public:
  virtual item_type type () const = 0;

  virtual QString name () const = 0;
  virtual QString namespace_uri () const = 0;
  virtual QString namespace_name () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;
};

#endif // ABSTRACT_ITEM_H
