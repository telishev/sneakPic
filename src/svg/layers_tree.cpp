#include "common/common_algorithms.h"
#include "common/debug_utils.h"

#include "svg/layers_tree.h"

#include "items/abstract_svg_item.h"
#include "attributes/svg_attributes_enum.h"

layers_tree::layers_tree (abstract_svg_item *root_item)
{
  m_root_item = root_item;
}

layers_tree::~layers_tree ()
{
}

bool layers_tree::add_item_with_check_internal (node *target_node, int global_id, layer_type type)
{
  if (target_node == nullptr)
    return false;

  target_node->children.emplace_back (new layers_tree_node (global_id, (int) target_node->children.size (), target_node, type));
  return true;
}

bool layers_tree::add_item_with_check (abstract_svg_item *item)
{
  if (item->parent () == m_root_item)
    return add_item_with_check_internal (&m_root, item->undo_id (), item->get_computed_attribute<svg_attribute_layer_type> ()->value ());
  else
    return add_item_with_check_internal (find_by_id (item->parent ()->undo_id ()), item->undo_id (), item->get_computed_attribute<svg_attribute_layer_type> ()->value ());
}

layers_tree::node *layers_tree::find_if (std::function <bool (const layers_tree::node *)> condition)
{
  return find_if_recursive (&m_root, condition);
}

void layers_tree::do_for_each_node (std::function <void (layers_tree::node *)> action)
{
  do_for_each_node_recursive (&m_root, action);
}

void layers_tree::do_for_each_node (std::function <void (const layers_tree::node *)> action) const
{
  do_for_each_node_recursive (&m_root, action);
}

void layers_tree::do_for_each_node_recursive (layers_tree::node *parent, std::function <void (layers_tree::node *)> action)
{
  if (parent->global_id != -1) // avoid root like fire
    action (parent);

  for (auto && node : parent->children)
    do_for_each_node_recursive (node.get (), action);
}

void layers_tree::do_for_each_node_recursive (const layers_tree::node *parent, std::function <void (const layers_tree::node *)> action) const
{
  if (parent->global_id != -1) // avoid root like water (cold)
    action (parent);

  for (auto && node : parent->children)
    do_for_each_node_recursive (node.get (), action);
}

layers_tree::node *layers_tree::find_if_recursive (layers_tree::node *parent, std::function <bool (const layers_tree::node *)> condition) const
{
  if (condition (parent))
    return parent;

  for (auto && node : parent->children)
    {
      auto found = find_if_recursive (node.get (), condition);
      if (found != nullptr)
        return found;
    }

  return nullptr;
}

layers_tree::node *layers_tree::find_by_id (int global_id)
{
  return find_if_recursive (&m_root, [=](const layers_tree::node *node_arg){ return node_arg->global_id == global_id; });
}

void layers_tree::clear ()
{
  m_root.children.clear (); // Everything will be cleared because of unique_ptrs
}

layers_tree_node * layers_tree::insert_node_after (layers_tree_node * target_node, int global_id, layer_type type)
{
  return insert_node_after (target_node, make_unique<layers_tree_node> (global_id, type));
}

layers_tree_node * layers_tree::insert_node_after (layers_tree_node * target_node, unique_ptr<layers_tree_node> node)
{
  if (target_node != nullptr)
    {
      auto parent = target_node->parent;
      int node_number = target_node->node_number;
      node->parent = parent;
      node->node_number = node_number + 1;
      for_each (parent->children.begin () + node_number + 1, parent->children.end (), [] (const unique_ptr <layers_tree_node> &node) { node->node_number++;});
      return parent->children.insert (parent->children.begin () + node_number + 1, std::move (node))->get ();
    }
  else
    {
      node->parent = &m_root;
      node->node_number = 0;
      m_root.children.push_back (std::move (node));
      return m_root.children.back ().get ();
    }
}

layers_tree_node * layers_tree::insert_node_before (layers_tree_node * target_node, int global_id, layer_type type)
{
  return insert_node_before (target_node, make_unique<layers_tree_node> (global_id, type));
}

layers_tree_node * layers_tree::insert_node_before (layers_tree_node * target_node, unique_ptr<layers_tree_node> node)
{
  if (target_node != nullptr)
    {
      auto parent = target_node->parent;
      int node_number = target_node->node_number;
      node->parent = parent;
      node->node_number = node_number;
      for_each (parent->children.begin () + node_number, parent->children.end (), [] (const unique_ptr <layers_tree_node> &node) { node->node_number++;});
      return parent->children.insert (parent->children.begin () + node_number, std::move (node))->get ();
    }
  else
    {
      node->parent = &m_root;
      node->node_number = 0;
      m_root.children.push_back (std::move (node));
      return m_root.children.back ().get ();
    }
}

layers_tree_node * layers_tree::insert_node_as_child (layers_tree_node *parent_node, int global_id, layer_type type)
{
  return insert_node_as_child (parent_node, make_unique<layers_tree_node> (global_id, type));
}

layers_tree_node * layers_tree::insert_node_as_child (layers_tree_node *parent_node, unique_ptr<layers_tree_node> node)
{
  node->parent = parent_node;
  node->node_number = (int) parent_node->children.size ();
  return parent_node->children.emplace (parent_node->children.end (), std::move (node))->get ();
}

layers_tree_node * layers_tree::find_closest_different_node (const layers_tree_node * m_active_layer_node) const
{
  auto parent = m_active_layer_node->parent;
  int node_number = m_active_layer_node->node_number;
  if (node_number < parent->children.size () - 1)
    return parent->children[node_number + 1].get ();
  else if (node_number > 0)
    return parent->children[node_number - 1].get ();
  else if (parent == &m_root)
    return nullptr;
  else
    return parent;
}

void layers_tree::remove_node (layers_tree_node *m_target_node)
{
  // here there are more complex operations
  auto parent = m_target_node->parent;
  // decreasing number of all subsequent nodes
  for_each (parent->children.begin () + m_target_node->node_number + 1, parent->children.end (), [](const unique_ptr <layers_tree_node> &node){ node->node_number--;});
  parent->children.erase (parent->children.begin () + m_target_node->node_number);
}

unique_ptr<layers_tree_node> layers_tree::take_out_node (layers_tree_node *m_target_node)
{
  // here there are more complex operations
  auto parent = m_target_node->parent;
  // decreasing number of all subsequent nodes
  for_each (parent->children.begin () + m_target_node->node_number + 1, parent->children.end (), [](const unique_ptr <layers_tree_node> &node){ node->node_number--;});
  auto temp = std::move (*(parent->children.begin () + m_target_node->node_number));
  parent->children.erase (parent->children.begin () + m_target_node->node_number);
  return std::move (temp);
}

int layers_tree::count () const
{
  int curCount = 0;
  do_for_each_node ([&](const layers_tree_node * /*node*/) {curCount++; });
  return curCount; // minus root which isn't actually layer
}

void layers_tree::move_node (layers_tree_node *from, layers_tree_node *to)
{
  DEBUG_ASSERT (from->parent == to->parent);
  auto parent = from->parent;
  int index_from = from->node_number;
  int index_to = to->node_number;
  slide (parent->children.begin () + index_from, parent->children.begin () + index_from + 1, parent->children.begin () + index_to);
  int counter = 0;
  for (auto &&n : parent->children)
    {
      n->node_number = counter++;
    }
}

layers_tree_node::layers_tree_node ()
{
  global_id = -1;
  node_number = 0;
  parent = nullptr;
  expanded = true;
  type = layer_type::LAYER;
}
