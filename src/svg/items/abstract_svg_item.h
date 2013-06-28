#ifndef BASE_SVG_ITEM_H
#define BASE_SVG_ITEM_H

#include <unordered_map>
#include <QString>

#include "svg/items/svg_item_type.h"
#include "svg/svg_namespaces.h"

#define SVG_ITEM                                   \
public:                                            \
virtual svg_item_type type () const override;      \
static QString static_name ();                     \
static svg_namespaces_t static_ns_type ();         \
static QString static_ns_URI ();                   \
virtual QString name () const override;            \
virtual svg_namespaces_t namespace_type () const;  \
private:                                           \

class QDomElement;
class QDomDocument;
class QPainter;

class abstract_attribute;
class svg_document;
class svg_attribute_id;
class svg_attribute_class;



enum class svg_namespaces_t;

class abstract_svg_item
{
  svg_document *m_document;

  abstract_svg_item *m_parent;
  abstract_svg_item *m_next_sibling;
  abstract_svg_item *m_prev_sibling;

  abstract_svg_item *m_first_child;
  abstract_svg_item *m_last_child;

  std::unordered_map<std::string, abstract_attribute *> m_attributes;

  svg_attribute_id *m_id;
  svg_attribute_class *m_class;

public:
  abstract_svg_item (svg_document *document);
  virtual ~abstract_svg_item ();

  virtual svg_item_type type () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;
  virtual QString name () const = 0;

  virtual QString namespace_uri () const;
  virtual QString namespace_name () const;

  virtual void draw (QPainter &/*painter*/) {};

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

  bool has_id () const { return m_id != nullptr; }
  QString id () const;
  void set_id (const QString &id);

  abstract_attribute *get_attribute (const QString &data) const;

  bool is_xml_class (const QString &class_name) const;

protected:
  virtual void process_attribute (abstract_attribute *attribute);

  void set_parent (abstract_svg_item *parent) { m_parent = parent; }
  void set_next_sibling (abstract_svg_item *next) { m_next_sibling = next; }
  void set_prev_sibling (abstract_svg_item *prev) { m_prev_sibling = prev; }
  QString full_name (const QString &namespace_name, const QString &local_name) const;

  void add_to_container ();
  void remove_from_container ();
};

#endif // BASE_SVG_ITEM_H

