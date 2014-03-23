#ifndef LAYERS_TREE_H
#define LAYERS_TREE_H

class abstract_svg_item;

struct layers_tree_node
{
  typedef layers_tree_node node;
  vector<unique_ptr <node>> children;
  node *parent;
  int global_id; // global id of the item
  int node_number; // should be updated on node creation
  bool expanded; // GUI property, is corresponding view item expanded

  node () { global_id = -1; node_number = 0; parent = nullptr; expanded = true; }
  node (int global_id_arg, int node_number_arg, node *parent_arg) : node () { global_id = global_id_arg; node_number = node_number_arg; parent = parent_arg; }
  node (int global_id_arg) : node () { global_id = global_id_arg; }
};

class layers_tree
{
  typedef layers_tree_node node;
private:
  node m_root; // root is always presented and doesn't contain any valid item.
  abstract_svg_item *m_root_item; // to ease the process of item addition

public:
  layers_tree (abstract_svg_item *root_item);
  ~layers_tree ();

  node *find_by_id (int global_id); // returns nullptr if item is not found
  bool add_item_with_check (abstract_svg_item *item); // function for pushing abstract_svg_item to back of their parent without exact knowledge if their parent item is valid layer
  layers_tree_node *insert_node_after (layers_tree_node * target_node, int global_id); // here we're adding the node with knowledge of global_id and where to place it in general structure
  layers_tree_node *insert_node_after (layers_tree_node * target_node, unique_ptr<layers_tree_node> node);
  layers_tree_node *insert_node_before (layers_tree_node * target_node, int global_id);
  layers_tree_node *insert_node_before (layers_tree_node * target_node, unique_ptr<layers_tree_node> node);
  layers_tree_node *insert_node_as_child (layers_tree_node *parent_node, int global_id);
  layers_tree_node * insert_node_as_child (layers_tree_node *parent_node, unique_ptr<layers_tree_node> node);
  node *find_if (std::function <bool (const node *)> condition);  // returns nullptr if item is not found
  node *root () { return &m_root; }
  void do_for_each_node (std::function <void (layers_tree::node *)> action);
  void do_for_each_node (std::function <void (const layers_tree::node *)> action) const;
  layers_tree_node * find_closest_different_node (const layers_tree_node * m_active_layer_node) const;
  void remove_node (layers_tree_node *m_target_node);
  unique_ptr<layers_tree_node> take_out_node (layers_tree_node *m_target_node);
  void clear ();
  int count () const;
  void move_node (layers_tree_node * from, layers_tree_node * to); // move node from 'from' to 'to'. They should have the same parent.

private:
  node *find_if_recursive (layers_tree::node *parent, std::function <bool (const node *)> condition) const;
  void do_for_each_node_recursive (layers_tree::node *parent, std::function <void (layers_tree::node *)> action);
  void do_for_each_node_recursive (const layers_tree::node *parent, std::function<void (const layers_tree::node *)> action) const;
  bool add_item_with_check_internal (node *target_node, int global_id);
  };

#endif // LAYERS_TREE_H

