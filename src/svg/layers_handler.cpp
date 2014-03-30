#include "gui/connection.h"
#include "svg/layers_handler.h"

#include "svg/svg_document.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"

#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attributes_string.h"
#include "svg/attributes/svg_attributes_enum.h"
#include "svg/items/svg_item_group.h"
#include "svg/undo/undo_handler.h"

#include "svg/layers_tree.h"

#include <QMetaObject>

layers_handler::layers_handler (svg_document *document)
{
  m_document = document;
  m_layers_tree.reset (new layers_tree (m_document->root ()));
  m_undo_handler = document->get_undo_handler ();
  CONNECT (m_document, &svg_document::undo_redo_done, this, &layers_handler::on_undo_redo_done);
  m_active_layer_node = nullptr;
  update_layer_tree (); // from items
  m_closest_to_active_global_id = -1;
  if (layers_count () == 0)
    add_new_layer (layer_type::LAYER, "Canvas");

  auto root = m_document->root ();
  rearrange_recursive (*root);
  emit layers_changed ();
}

void layers_handler::rearrange_recursive (abstract_svg_item &parent_item)
{
  vector <pair <abstract_svg_item *, abstract_svg_item *> > move_child_to_layer;
  string layer_attribute_name = svg_attribute_layer_name ().type_name ();
  auto parent_node = m_layers_tree->find_by_id (parent_item.undo_id ());
  if (parent_node == nullptr)
    return;

  auto closest_layer_iterator = find_if (parent_node->children.begin (), parent_node->children.end (), [] (const unique_ptr<layers_tree_node> &node) { return node->type == layer_type::LAYER; });
  abstract_svg_item *closest_layer_item = nullptr;
  if (closest_layer_iterator != parent_node->children.end ())
    closest_layer_item = get_layer_item ((*closest_layer_iterator).get ());

  add_new_layer (layer_type::LAYER , QString (), &parent_item);

  for (auto && child : parent_item)
    {
      if (!child->has_attribute (layer_attribute_name))
        {
          if (child->get_computed_attribute<svg_attribute_layer_type> ()->value () != layer_type::FOLDER)
            {
              if (closest_layer_item == nullptr)
                {
                  add_new_layer (layer_type::LAYER, QString (), &parent_item);
                  closest_layer_item = get_layer_item (parent_node->children.back ().get ());
                }

              move_child_to_layer.push_back ({child, closest_layer_item});
            }
          else
            {
              rearrange_recursive (*child);
            }
        }
      else
        closest_layer_item = child;
    }

  for (auto && action : move_child_to_layer)
    {
      parent_item.make_orphan (action.first);
      action.second->adopt_orphan (action.first);
    }
}

void layers_handler::on_undo_redo_done ()
{
  update_layer_tree ();
  emit active_layer_changed ();
}

void layers_handler::add_layer_items_recursive (abstract_svg_item *item)
{
  const string attribute_name = svg_attribute_layer_name ().type_name ();
  if (item->has_attribute (attribute_name))
    {
      if (!m_layers_tree->add_item_with_check (item))
        item->remove_attribute (item->get_computed_attribute <svg_attribute_layer_name> ()); // If layer cannot be added for some reasons then remove it's layer tag
    }
  if (item->get_computed_attribute<svg_attribute_layer_type> ()->value () == layer_type::FOLDER || item == m_document->root ())
    {
      for (int i = item->children_count () - 1; i >= 0; i--)
        add_layer_items_recursive (item->child (i));
    }
}

void layers_handler::update_layer_tree ()
{
  auto container = m_document->item_container ();
  m_layers_tree->clear ();

  // for now "layer" groups are determined by existence of "layer-name" attribute

  add_layer_items_recursive (container->get_root ());

  update_active_layer_node_by_attribute ();

  emit layers_changed ();
}

void layers_handler::add_new_layer (layer_type layer_type_arg, QString name /*= QString ()*/, abstract_svg_item *override_parent /*= nullptr*/)
{
  if (name.isNull ())
    {
      QRegExp re = (layer_type_arg == layer_type::LAYER ? QRegExp ("^Layer (\\d+)$") : QRegExp ("^Folder (\\d+)$"));
      set<int> numbers;

      m_layers_tree->do_for_each_node ([&] (layers_tree_node * l)
      {
        bool ok;

        // we're being overly sure that this undoable is svg_abstract_item
        if (re.exactMatch (static_cast <abstract_svg_item *> (m_undo_handler->get_item (l->global_id))->get_computed_attribute<svg_attribute_layer_name> ()->value ()))
          {
            int num = re.capturedTexts ()[1].toInt (&ok);
            if (ok)
              numbers.insert (num);
          }
      });
      int prev_num = 0, free_num = [&]() { if (numbers.empty ()) return 1; auto f = numbers.end (); f--; return *f + 1; }();
      for (auto && x : numbers)
        {
          if (x - prev_num > 1)
            {
              free_num = prev_num + 1;
              break;
            }
          prev_num = x;
        }
      name = (layer_type_arg == layer_type::LAYER ? QString ("Layer %1").arg (free_num) : QString ("Folder %1").arg (free_num));
    }

  auto *layer_item = m_document->create_new_svg_item<svg_item_group> (true); // ignoring check
  layer_item->get_attribute_for_change<svg_attribute_layer_name> ()->set_value (name);
  layer_item->get_attribute_for_change<svg_attribute_display> ()->set_value (display::INLINE); // Make sure that layer has visibility attribute in a moment of creation
  layer_item->get_attribute_for_change<svg_attribute_layer_type> ()->set_value (layer_type_arg);
  if (override_parent == nullptr)
    {
      auto active_layer_item = get_active_layer_item ();
      auto parent_item = (active_layer_item != nullptr ? active_layer_item->parent () : m_document->root ());
      parent_item->push_back (layer_item);
      if (active_layer_item != nullptr)
        parent_item->move_child (active_layer_item->child_index () + 1, layer_item);

      m_active_layer_node = m_layers_tree->insert_node_before (m_active_layer_node, layer_item->undo_id (), layer_item->get_computed_attribute<svg_attribute_layer_type> ()->value ());
      update_attribute_by_active_layer_node ();

      emit layers_changed ();
    }
  else
    {
      override_parent->push_back (layer_item);
      m_active_layer_node = m_layers_tree->insert_node_as_child (m_layers_tree->find_by_id (override_parent->undo_id ()), layer_item->undo_id (), layer_item->get_computed_attribute<svg_attribute_layer_type> ()->value ());
    }
};

void layers_handler::remove_active_layer ()
{
  if (layers_count () == 1)
    return;

  auto close_node = m_layers_tree->find_closest_different_node (m_active_layer_node);
  if (close_node == nullptr)
    return;

  auto item = get_active_layer_item ();
  item->parent ()->remove_child (item);

  m_layers_tree->remove_node (m_active_layer_node);
  m_active_layer_node = close_node;
  update_attribute_by_active_layer_node ();
  emit layers_changed ();
  m_document->apply_changes ("Remove Active Layer");
  m_document->emit_items_changed ();
};

layers_handler::~layers_handler ()
{

}

QString layers_handler::get_layer_name (const QModelIndex &index) const
{
  return get_layer_item (index)->get_computed_attribute <svg_attribute_layer_name> ()->value ();
}

abstract_svg_item * layers_handler::get_layer_item (const QModelIndex &index) const
{
  auto node = get_node_by_index (index);
  return get_layer_item (node);
}

layers_tree_node *layers_handler::get_node_by_index (const QModelIndex &index) const
{
  return reinterpret_cast<layers_tree_node *> (index.internalPointer ());
}

abstract_svg_item * layers_handler::get_layer_item (const layers_tree_node *node) const
{
  return node != nullptr ? static_cast <abstract_svg_item *> (m_undo_handler->get_item (node->global_id)) : nullptr;
}

abstract_svg_item *layers_handler::get_active_layer_item () const
{
  return m_active_layer_node ? static_cast<abstract_svg_item *> (m_undo_handler->get_item (m_active_layer_node->global_id)) : nullptr;
}

int layers_handler::layers_count ()
{
  return (int) m_layers_tree->count ();
}

void layers_handler::toggle_layer_visibility (const QModelIndex &layer_index)
{
  auto item = get_layer_item (layer_index);

  {
    auto attr = item->get_attribute_for_change<svg_attribute_display> ();
    if (attr->value () != display::NONE)
      attr->set_value (display::NONE);
    else
      attr->set_value (display::INLINE);
  }

  m_document->apply_changes ("Toggle Layer Visibility");
}

void layers_handler::set_active_layer (const QModelIndex &new_index)
{
  m_active_layer_node = reinterpret_cast <layers_tree_node *> (new_index.internalPointer ());
  update_attribute_by_active_layer_node ();
  m_document->apply_changes ("Change Active Layer");
  emit active_layer_changed ();
}

layers_tree_node *layers_handler::active_layer_node ()
{
  return m_active_layer_node;
}

void layers_handler::update_active_layer_node_by_attribute ()
{
  auto item_container = m_document->item_container ();
  auto active_layer_attr = item_container->get_root ()->get_computed_attribute<svg_attribute_active_layer> ();
  QString active_layer_name = active_layer_attr ? active_layer_attr->value () : QString ();
  if (active_layer_name.isNull ())
    {
      m_active_layer_node = nullptr;
      return;
    }

  auto item = find_if (item_container->begin (), item_container->end (), [&](abstract_svg_item *item){ return item->get_computed_attribute<svg_attribute_layer_name> ()->value () == active_layer_name; });
  if (item == item_container->end ())
    {
      if (m_layers_tree->root ()->children.size () > 0)
        m_active_layer_node = m_layers_tree->root ()->children[0].get ();
      return;
    }

  m_active_layer_node = m_layers_tree->find_by_id ((*item)->undo_id ());
}

void layers_handler::update_attribute_by_active_layer_node ()
{
  auto item_container = m_document->item_container ();
  auto attr = item_container->get_root ()->get_attribute_for_change<svg_attribute_active_layer> ();
  attr->set_value (get_active_layer_item ()->get_computed_attribute<svg_attribute_layer_name> ()->value ());
}

bool layers_handler::is_layer_visible (const QModelIndex &index)
{
  return get_layer_item (index)->get_computed_attribute<svg_attribute_display> ()->value () != display::NONE;
}

layer_type layers_handler::get_layer_type (const QModelIndex &index, layer_type default_type)
{
  auto item = get_layer_item (index);
  return item != nullptr ? item->get_computed_attribute<svg_attribute_layer_type> ()->value () : default_type;
}

void layers_handler::rename_layer (const QModelIndex &index, QString new_name)
{
  auto item = get_layer_item (index);
  if (item != m_document->root ())
    {
      item->get_attribute_for_change<svg_attribute_layer_name> ()->set_value (new_name);
      m_document->apply_changes ("Rename Layer");
    }
}

void layers_handler::move_layer_inside (layers_tree_node *from, layers_tree_node *to)
{
  if (to == nullptr || from == nullptr || from->parent == to || to->type == layer_type::LAYER)
    return;

  auto cur_parent = to->parent;
  while (cur_parent != nullptr) // if to is descendant of from then this move isn't possible
    {
      if (cur_parent == from)
        return;
      cur_parent = cur_parent->parent;
    }

  auto item_from = get_layer_item (from);
  auto item_to = get_layer_item (to);
  item_from->parent ()->make_orphan (item_from);
  item_to->adopt_orphan (item_from);

  m_layers_tree->insert_node_as_child (to, m_layers_tree->take_out_node (from));
  m_document->apply_changes ("Layer Moved");
  update_attribute_by_active_layer_node ();
  emit layers_changed ();
}

void layers_handler::move_layer (layers_tree_node *from, layers_tree_node *to, bool before_first_one /*= false*/)
{
  if (to == nullptr || from == nullptr || from == to)
    return;

  auto cur_parent = to->parent;
  while (cur_parent != nullptr) // if to is descendant of from then this move isn't possible
    {
      if (cur_parent == from)
        return;
      cur_parent = cur_parent->parent;
    }

  auto item_from = get_layer_item (from);
  abstract_svg_item *item_to_parent = nullptr;
  if (before_first_one)
    {
      item_to_parent = get_layer_item (to);
      if (item_to_parent == nullptr)
        item_to_parent = m_document->root ();
    }
  else
    item_to_parent = get_layer_item (to)->parent ();

  item_from->parent ()->make_orphan (item_from);
  item_to_parent->adopt_orphan (item_from);
  item_to_parent->move_child (before_first_one ? 0 : get_layer_item (to)->child_index () + 1, item_from);

  if (before_first_one)
    m_layers_tree->insert_node_after (to->children.back ().get (), m_layers_tree->take_out_node (from));
  else if (from->parent == to->parent)
    m_layers_tree->move_node (from, to);
  else
    m_layers_tree->insert_node_before (to, m_layers_tree->take_out_node (from));

  m_document->apply_changes ("Layer Moved");
  update_attribute_by_active_layer_node ();
  emit layers_changed ();
}

int layers_handler::get_layer_opacity (const layers_tree_node *node)
{
  auto item = get_layer_item (node);
  return (int) (item->get_computed_attribute<svg_attribute_opacity> ()->value () * 100.0);
}

int layers_handler::get_layer_opacity (const QModelIndex &index)
{
  auto item = get_layer_item (index);
  return (int) (item->get_computed_attribute<svg_attribute_opacity> ()->value () * 100.0);
}

int layers_handler::get_active_layer_opacity ()
{
  return get_layer_opacity (m_active_layer_node);
}

void layers_handler::set_active_layer_opacity (int value)
{
  auto item = get_active_layer_item ();
  item->get_attribute_for_change<svg_attribute_opacity> ()->set_value ((double) value / 100.0);
  m_document->apply_changes ("Change Layer Opacity");
}

void layers_handler::add_new_layer_slot ()
{
  add_new_layer (layer_type::LAYER);
  m_document->apply_changes ("Add New Layer");
}

void layers_handler::add_new_folder_slot ()
{
  add_new_layer (layer_type::FOLDER);
  m_document->apply_changes ("Add New Folder");
}

layers_tree_node *layers_handler::tree_root ()
{
  return m_layers_tree->root ();
}
