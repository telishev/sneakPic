#ifndef BASE_SVG_ITEM_H
#define BASE_SVG_ITEM_H

#include <unordered_set>
#include <QString>

#include "svg/items/svg_item_type.h"

#define SVG_ITEM(NAME,TYPE,NS_TYPE)                                                     \
public:                                                                                 \
virtual svg_item_type type () const override { return TYPE; }                           \
static QString static_name () { return NAME; }                                          \
static svg_namespaces_t static_ns_type () { return NS_TYPE; }                           \
static QString static_ns_URI () { return svg_namespaces::uri (static_ns_type ()); }     \
virtual QString name () const override { return static_name (); }                       \
virtual svg_namespaces_t namespace_type () const { return static_ns_type (); }          \
private:                                                                                \

class QDomElement;

class abstract_attribute;
class QDomDocument;
class svg_document;

enum class svg_namespaces_t;

class abstract_svg_item
{
  svg_document *m_document;

  abstract_svg_item *m_parent;
  abstract_svg_item *m_next_sibling;
  abstract_svg_item *m_prev_sibling;

  abstract_svg_item *m_first_child;
  abstract_svg_item *m_last_child;

  std::unordered_set<abstract_attribute *> m_attributes;

public:
  abstract_svg_item (svg_document *document);
  virtual ~abstract_svg_item ();

  virtual svg_item_type type () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;
  virtual QString name () const = 0;

  virtual QString namespace_uri () const;
  virtual QString namespace_name () const;

  svg_document *document () const { return m_document; }

  void add_attribute (abstract_attribute *attribute);
  void remove_attribute (abstract_attribute *attribute);

  abstract_svg_item *parent () const { return m_parent; }
  abstract_svg_item *next_sibling () const { return m_next_sibling; }
  abstract_svg_item *prev_sibling () const { return m_prev_sibling; }

  abstract_svg_item *first_child () const { return m_first_child; }
  abstract_svg_item *last_child () const { return m_last_child; }

  void insert_child (abstract_svg_item *position, abstract_svg_item *new_child);
  void remove_child (abstract_svg_item *child);

  void read (const QDomElement &item);
  void write (QDomElement &item, QDomDocument &doc) const;

protected:
  virtual void process_attribute (abstract_attribute * /*attribute*/) {}

  void set_parent (abstract_svg_item *parent) { m_parent = parent; }
  void set_next_sibling (abstract_svg_item *next) { m_next_sibling = next; }
  void set_prev_sibling (abstract_svg_item *prev) { m_prev_sibling = prev; }
  QString full_name (const QString &namespace_name, const QString &local_name) const;
};

#endif // BASE_SVG_ITEM_H

