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
    add_new_layer ("Canvas");

  auto root = m_document->root ();
  string attribute_name = svg_attribute_layer_name ().type_name ();
  auto closest_layer = get_layer_item (m_layers_tree->root ()->children[0].get ()); // There should be at least "Canvas" layer
  vector <pair <abstract_svg_item *, abstract_svg_item *> > move_child_to_layer;

  for (auto && child : *root)
    {
      if (!child->has_attribute (attribute_name))
        {
          move_child_to_layer.push_back ({child, closest_layer});
        }
      else
        closest_layer = child;
    }

  for (auto && action : move_child_to_layer)
    {
      root->make_orphan (action.first);
      action.second->adopt_orphan (action.first);
    }
  emit layers_changed ();
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
  for (int i = item->children_count () - 1; i >= 0; i--)
    add_layer_items_recursive (item->child (i));
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

void layers_handler::add_new_layer (QString name)
{
  if (name.isNull ())
    {
      QRegExp re ("^Layer (\\d+)$");
      set<int> numbers;

      m_layers_tree->do_for_each_node ([&](layers_tree_node *l)
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
      name = QString ("Layer %1").arg (free_num);
    }

  auto *layer_item = m_document->create_new_svg_item<svg_item_group> ();
  layer_item->get_attribute_for_change<svg_attribute_layer_name> ()->set_value (name);
  m_document->root ()->push_back (layer_item);

  auto active_layer_item = get_active_layer_item ();
  if (active_layer_item != nullptr)
    m_document->root ()->move_child (active_layer_item->child_index () + 1, layer_item);
  else
    m_document->root ()->move_child (0, layer_item);

  m_active_layer_node = m_layers_tree->insert_node_before (m_active_layer_node, layer_item->undo_id ());

  update_attribute_by_active_layer_node ();

  emit layers_changed ();
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
  return static_cast <abstract_svg_item *> (m_undo_handler->get_item (node->global_id));
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

  auto item = std::find_if (item_container->begin (), item_container->end (), [&](abstract_svg_item *item){ return item->get_computed_attribute<svg_attribute_layer_name> ()->value () == active_layer_name; });
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
  return true;
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
  if (to == nullptr || from == nullptr || from->parent == to)
    return;

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
  if (to == nullptr || from == nullptr)
    return;

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
    m_layers_tree->insert_node_after (to, m_layers_tree->take_out_node (from));

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
  add_new_layer ();
  m_document->apply_changes ("Add New Layer");
}

layers_tree_node *layers_handler::tree_root ()
{
  return m_layers_tree->root ();
}
