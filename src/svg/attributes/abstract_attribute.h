#ifndef ABSTRACT_ATTRIBUTE_H
#define ABSTRACT_ATTRIBUTE_H

#include "svg/undo/undoable.h"

#include <string>

#define SVG_ATTRIBUTE                                           \
public:                                                         \
  static const char *static_type_name ();                       \
  static svg_namespaces_t static_ns_type ();                    \
  virtual svg_attribute_type type () const override;            \
  static svg_attribute_type static_type ();                     \
  static const char *static_ns_URI ();                          \
  virtual svg_namespaces_t namespace_type () const override;    \
  virtual const char *namespace_uri () const override;          \
  virtual const char *namespace_name () const override;         \
  virtual const char *type_name () const override;              \
  virtual svg_inherit_type inherit_type () const override;      \
  static svg_inherit_type static_inherit_type ();               \
  static const abstract_attribute *static_default_value ();     \
  virtual const abstract_attribute *default_value () const override;\
private:                                                        \

class items_edit_handler_t;
class abstract_svg_item;

class QString;

enum class svg_namespaces_t;
enum class svg_attribute_type;
enum class svg_inherit_type;

class abstract_attribute : public undoable
{
  items_edit_handler_t *m_edit_handler;
  int m_item_id;
  bool m_is_inherited;
public:
  abstract_attribute ();
  virtual ~abstract_attribute ();

  virtual bool read (const char *data, bool from_css = false) = 0;
  virtual bool write (QString &data, bool to_css = false) const = 0;

  virtual const char *type_name () const = 0;
  virtual const char *namespace_uri () const = 0;
  virtual const char *namespace_name () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;

  virtual svg_attribute_type type () const = 0;
  virtual svg_inherit_type inherit_type () const = 0;
  virtual const abstract_attribute *default_value () const = 0;

  virtual abstract_attribute *clone () const;

  bool is_empty () const;

  void set_item (int item_id);
  void set_edit_handler (items_edit_handler_t *edit_handler) { m_edit_handler = edit_handler; }
  int item_id () const { return m_item_id; }
  abstract_svg_item *item () const;
  void set_is_inherited (bool value) { m_is_inherited = value; }
  bool is_inherited () const { return m_is_inherited; }

  virtual abstract_state_t *create_state () override;
  virtual void load_from_state (const abstract_state_t *state) override;

  void register_change ();
};

#endif // ABSTRACT_ATTRIBUTE_H
