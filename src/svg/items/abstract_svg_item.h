#ifndef BASE_SVG_ITEM_H
#define BASE_SVG_ITEM_H

#include <map>
#include <utility>
#include <functional>
#include <vector>

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/attribute_pointer.h"
#include "svg/undo/undoable.h"

#define SVG_ITEM                                   \
public:                                            \
virtual svg_item_type type () const override;      \
static svg_item_type static_type ();               \
static const char *static_type_name ();            \
static svg_namespaces_t static_ns_type ();         \
static const char *static_ns_URI ();               \
virtual const char *type_name () const override;   \
virtual svg_namespaces_t namespace_type () const;  \
private:                                           \

class abstract_attribute;
class svg_document;
class svg_item_defs;
class svg_graphics_item;
class abstract_attribute_pointer;
class svg_item_observer;
class undo_handler;

enum class svg_namespaces_t;
enum class svg_item_type;
enum class svg_inherit_type;
enum class overlay_item_type;
enum class svg_attribute_type;

class abstract_svg_item : public undoable
{
  svg_document *m_document;
  map<string, int> m_attributes;

  string m_original_id;
  string m_own_id;

  vector<int> m_children;
  int m_parent;

  vector<int> *m_observers; ///< observers that should be informed on some changes
  vector<int> *m_created_observers; ///< observers created by me

public:
  abstract_svg_item (svg_document *document);
  virtual ~abstract_svg_item ();

  int child_index () const;
  int children_count () const;
  abstract_svg_item *parent () const;
  abstract_svg_item *child (int index) const;
  void push_back (abstract_svg_item *new_child);
  void insert_child (int index, abstract_svg_item *new_child);
  void remove_child (abstract_svg_item *child);
  void move_child (int index, abstract_svg_item *child);

  virtual svg_item_type type () const = 0;
  virtual svg_namespaces_t namespace_type () const = 0;
  virtual const char *type_name () const = 0;

  virtual const char *namespace_uri () const;
  virtual const char *namespace_name () const;

  svg_document *document () const { return m_document; }

  void add_attribute (abstract_attribute *attribute);
  void remove_attribute (const abstract_attribute *attribute);

  bool has_name () const;
  string name () const;

  /// returns attribute for the current item
  template <typename T>
  attribute_pointer<T> get_attribute_for_change ()
  {
    signal_attribute_change_start (get_computed_attribute<T> ());
    abstract_attribute *attribute = get_attribute_for_change (T::static_type_name (), T::static_inherit_type (), T::static_type (), T::default_value ());
    return std::move (attribute_pointer<T> (static_cast<T *>(attribute), this));
  }

  /// returns attribute with respect to styling and css
  template <typename T>
  const T *get_computed_attribute (const T *default_val = static_cast<const T *> (T::default_value ())) const
  {
    const abstract_attribute *attribute = get_computed_attribute (T::static_type_name (), T::static_inherit_type (), T::static_type ());
    /// if not found, return default value
    if (!attribute || attribute->type () != T::static_type ())
      attribute = default_val;

    return static_cast<const T *>(attribute);
  }

  bool has_attribute (const string &type_name) const;

  /// checks for correctness
  bool process_after_read ();

  void update_name ();

  bool is_cloned () const;
  /// creates cloned item, for "use" item
  abstract_svg_item *create_clone ();

  void register_item_name ();

  virtual const svg_graphics_item *to_graphics_item () const { return nullptr; }
  virtual svg_graphics_item *to_graphics_item () { return nullptr; }

  bool is_character_data () const;

  void observe_item (abstract_svg_item *item_to_observe, svg_item_observer *observer);
  void erase_created_observer (svg_item_observer *observer);

  vector<const abstract_attribute *> attributes_list () const;
  abstract_svg_item *sibling (int index) const;

  void create_unique_name ();

  class iterator : public std::iterator<std::forward_iterator_tag, abstract_svg_item *>
  {
    vector<int>::iterator m_it;
    undo_handler *m_undo_handler;

  private:
    iterator (vector<int>::iterator it, undo_handler *undo_handler_arg) { m_it = it; m_undo_handler = undo_handler_arg; }
    iterator () {  }

  public:
    abstract_svg_item *operator* ();
    bool operator != (const iterator &other) const { return m_it != other.m_it; };
    bool operator == (const iterator &other) const { return m_it == other.m_it; };
    iterator &operator++() { ++this->m_it; return *this; };

    friend class abstract_svg_item;
  };

  iterator begin ();
  iterator end ();

  void make_orphan (abstract_svg_item *child);  // WARNING: These function does not generate proper undo, so you'd better use it in the stage before undo has any sense.
  void adopt_orphan (abstract_svg_item *child); // --- / ---

protected:
  virtual bool process_item_after_read () { return true; }

  virtual abstract_state_t *create_state () override;
  virtual void load_from_state (const abstract_state_t *state) override;

private:
  void add_to_container ();
  void remove_from_container ();
  const abstract_attribute *get_computed_attribute (const char *data, svg_inherit_type inherit_type, svg_attribute_type attr_type) const;
  const abstract_svg_item *get_original_item () const;
  void create_id_by_attr ();
  abstract_attribute *get_attribute_for_change (const char *data, svg_inherit_type inherit_type, svg_attribute_type attr_type, const abstract_attribute *default_val);
  abstract_attribute *get_attribute (const char *data, bool get_clone_attributes) const;

  void signal_child_inserted (const string &child, int position);
  void signal_child_removed (const string &child_name, int pos);
  void signal_item_removed ();
  void signal_child_moved (const string &child_name, int old_pos, int new_pos);
  void signal_attribute_change_start (const abstract_attribute *attribute);
  void signal_attribute_change_end (const abstract_attribute *attribute);
  void register_item_change ();
  void add_observer (svg_item_observer *observer);
  abstract_attribute *get_attribute_by_id (int id) const;
  abstract_svg_item *get_item_by_id (int id) const;
  void send_to_listeners (std::function< void (svg_item_observer *)> func);

  void prepare_to_remove ();

  friend class cloned_item_observer;
  friend class abstract_attribute_pointer;
};

#endif // BASE_SVG_ITEM_H

