#ifndef LAYERS_HANDLER_H
#define LAYERS_HANDLER_H

#include <QObject>

class abstract_svg_item;
class svg_document;
class svg_items_container;
class layers_tree;
struct layers_tree_node;
class undo_handler;
enum class layer_type;

class layers_handler : public QObject
{
  Q_OBJECT

  unique_ptr<layers_tree> m_layers_tree;
  svg_document *m_document;
  layers_tree_node *m_active_layer_node;
  int m_closest_to_active_global_id;
  undo_handler *m_undo_handler;
  abstract_svg_item *get_layer_item (const QModelIndex &index) const;
  abstract_svg_item *get_layer_item (const layers_tree_node *node) const;

public:
  layers_handler (svg_document *document);
  ~layers_handler ();
  QString get_layer_name (const QModelIndex &index) const;
  int layers_count();
  layers_tree_node *active_layer_node ();
  abstract_svg_item *get_active_layer_item () const;
  bool is_layer_visible (const QModelIndex &index ) const;
  bool is_layer_locked (const QModelIndex &index ) const;
  void toggle_layer_visibility (const QModelIndex &layer_index);
  void toggle_layer_is_locked (const QModelIndex &layer_index);
  void rename_layer(const QModelIndex &index, QString new_name);
  void move_layer (layers_tree_node *from, layers_tree_node *to, bool before_first_one = false); // if after_last_one is true then to is parent of item where we should move
  void move_layer_inside (layers_tree_node *from, layers_tree_node *to);
  int get_active_layer_opacity ();
  bool get_active_layer_is_locked () const;
  void set_active_layer_opacity (int value);
  int get_layer_opacity (const QModelIndex &index);
  int get_layer_opacity (const layers_tree_node *node);
  layers_tree_node *tree_root ();
  void rearrange_recursive (abstract_svg_item &parent_item); // make folders and root empty of items if they have any + create layers and move items to them if needed
  layer_type get_layer_type (const QModelIndex &index, layer_type default_type);

private:
  void update_layer_tree ();
  void update_active_layer_node_by_attribute();
  void update_attribute_by_active_layer_node ();
  void add_new_layer (layer_type layer_type_arg, QString name = QString (), abstract_svg_item *override_parent = nullptr); // override_parent to use this function also initial arranging
  layers_tree_node *get_node_by_index (const QModelIndex &index) const;
  void add_layer_items_recursive (abstract_svg_item *item);

public slots:
  void set_active_layer (const QModelIndex &new_index );
  void add_new_layer_slot ();
  void add_new_folder_slot ();
  void remove_active_layer ();

private slots:
  void on_undo_redo_done ();

signals:
  void layers_changed (); // for non-obvious cases (like undo)
  void active_layer_changed ();
};

#endif //LAYERS_HANDLER_H
