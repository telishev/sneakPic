#ifndef BASE_SVG_ITEM_H
#define BASE_SVG_ITEM_H

#include <unordered_map>
#include <QString>

#include "common/tree_structure.h"

#define SVG_ITEM                                   \
public:                                            \
virtual svg_item_type type () const override;      \
static const char *static_name ();                 \
static svg_namespaces_t static_ns_type ();         \
static const char *static_ns_URI ();               \
virtual const char *name () const override;        \
virtual svg_namespaces_t namespace_type () const;  \
private:                                           \

class QDomElement;
class QDomDocument;

class abstract_attribute;
class svg_document;
class svg_item_defs;
class abstract_renderer_item;

enum class svg_namespaces_t;
enum class svg_item_type;

class abstract_svg_item : public tree_structure<abstract_svg_item>
{
  svg_document *m_document;
  std::unordered_map<std::string, abstract_attribute *> m_attributes;

public:
  abstract_svg_item (svg_document *document);
  virtual ~abstract_svg_item ();

  virtual svg_item_type type () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;
  virtual const char *name () const = 0;

  virtual const char *namespace_uri () const;
  virtual const char *namespace_name () const;

  virtual void update_renderer_item () {};
  virtual const abstract_renderer_item *get_renderer_item () const { return nullptr; }

  /// return true if there is a need to render children of this item
  virtual bool render_children () const { return 0; }

  svg_document *document () const { return m_document; }

  void add_attribute (abstract_attribute *attribute);
  void remove_attribute (abstract_attribute *attribute);

  void read (const QDomElement &item);
  void write (QDomElement &item, QDomDocument &doc) const;

  bool has_id () const;
  QString id () const;

  bool is_xml_class (const QString &class_name) const;

  /// returns attribute for the current item
  template <typename T>
  T *get_attribute () const
  {
    return static_cast< T *> (get_attribute (T::static_name ()));
  }

  /// returns attribute with respect to styling and css
  template <typename T>
  const T *get_computed_attribute () const
  {
    const abstract_attribute *attribute = get_computed_attribute (T::static_name (), T::static_is_styleable ());
    /// if not found, return default value
    if (!attribute)
      attribute = T::default_value ();

    return static_cast<const T *>(attribute);
  }

  /// checks for correctness
  bool check ();

protected:
  virtual bool read_item (const QString &/*data*/) { return true; }
  virtual bool check_item () = 0;

private:
  QString full_name (const QString &namespace_name, const QString &local_name) const;
  void add_to_container ();
  void remove_from_container ();
  abstract_attribute *get_attribute (const char *data) const;
  const abstract_attribute *get_computed_attribute (const char *data, bool is_styleable) const;
  const abstract_attribute *find_attribute_in_selectors (const char *data, const abstract_svg_item *item) const;
  const abstract_attribute *find_attribute_in_style_item (const char *data, const abstract_svg_item *item) const;
};

#endif // BASE_SVG_ITEM_H

