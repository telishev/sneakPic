#include "gui/connection.h"
#include "svg/layers_handler.h"

#include "svg/svg_document.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"

#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attributes_string.h"
#include "svg/attributes/svg_attributes_enum.h"
#include "svg/items/svg_item_group.h"

#include <QMetaObject>

layers_handler::layers_handler (svg_document *document)
{
  m_layers_container.clear ();
  m_document = document;
  CONNECT (m_document, &svg_document::undo_redo_done, this, &layers_handler::on_undo_redo_done);
  update_layer_list (); // from items
  if (layers_count () == 0)
    add_new_layer ("Canvas");

  auto root = m_document->root ();
  string attribute_name = svg_attribute_layer_name ().type_name ();
  auto closest_layer = get_layer_item (0); // There should be at least "Canvas" layer
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
}

void layers_handler::on_undo_redo_done ()
{
  update_layer_list ();
  emit active_layer_changed ();
}

void layers_handler::update_layer_list ()
{
  auto root = m_document->root ();
  auto it = root->begin ();
  m_layers_container.clear ();
  // for now "layer" groups are determined by existence of "layer-name" attribute

  string attribute_name = svg_attribute_layer_name ().type_name ();
  while (true)
    {
      it = find_if (it, root->end (), [&] (abstract_svg_item * item) { return item->has_attribute (attribute_name); });
      if (it != root->end ())
        m_layers_container.push_back ((*it)->name ());
      else
        break;
      ++it;
    }

  std::sort (m_layers_container.begin (), m_layers_container.end (), [&] (const string & layerA, const string & layerB)
  {
    auto item_container = m_document->item_container ();
    return item_container->get_item (layerA)->child_index () > item_container->get_item (layerB)->child_index ();
  });

  update_active_layer_index_by_attribute ();

  emit layers_changed ();
}

void layers_handler::add_new_layer (QString name)
{
  if (name.isNull ())
  {
    QRegExp re ("^Layer (\\d+)$");
    set<int> numbers;

    for (auto && l : *this)
    {
      bool ok;
      if (re.exactMatch (l->get_computed_attribute<svg_attribute_layer_name> ()->value ()))
      {
        int num = re.capturedTexts ()[1].toInt (&ok);
        if (ok)
          numbers.insert (num);
      }
    }
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
  if (active_layer_item != m_document->root ())
    m_document->root ()->move_child (active_layer_item->child_index () + 1, layer_item);
  else
    m_document->root ()->move_child (0, layer_item);

  m_layers_container.insert (m_layers_container.begin () + (m_active_layer_index >= 0 ? m_active_layer_index : m_layers_container.size ()), layer_item->name ());

  update_attribute_by_active_layer_index ();

  emit layers_changed ();
};

void layers_handler::remove_active_layer ()
{
  if (layers_count ())
    return;

  auto item = get_active_layer_item ();
  item->parent ()->remove_child (item);
  m_layers_container.erase (m_layers_container.begin () + m_active_layer_index);
  update_attribute_by_active_layer_index ();
  emit layers_changed ();
  m_document->apply_changes ("Remove Active Layer");
  m_document->emit_items_changed ();
};

layers_handler::~layers_handler ()
{

}

QString layers_handler::get_layer_name (int index) const
{
  return get_layer_item (index)->get_computed_attribute <svg_attribute_layer_name> ()->value ();
}

abstract_svg_item *layers_handler::get_layer_item (int index) const
{
  return (index >= 0 && index < m_layers_container.size ()) ? m_document->item_container ()->get_item (m_layers_container[index]) : m_document->root ();
}

abstract_svg_item *layers_handler::get_active_layer_item () const
{
  return get_layer_item (m_active_layer_index);
}

abstract_svg_item *layers_handler::layers_iterator::operator* ()
{
  return m_container->get_item (*m_it);
}

layers_handler::layers_iterator::layers_iterator (svg_items_container *container, container_type::iterator it)
{
  m_container = container;
  m_it = it;
}

bool layers_handler::layers_iterator::operator!= (const layers_iterator &other) const
{
  return (m_it != other.m_it);
}

bool layers_handler::layers_iterator::operator== (const layers_iterator &other) const
{
  return (m_it == other.m_it);
}


layers_handler::layers_iterator &layers_handler::layers_iterator::operator++ ()
{
  this->m_it++;
  return *this;
}

int layers_handler::layers_iterator::operator- (const layers_iterator &other) const
{
  return (m_it - other.m_it);
}

layers_handler::layers_iterator layers_handler::layers_iterator::operator+ (int add) const
{
  return layers_handler::layers_iterator (m_container, m_it + add);
}

layers_handler::layers_iterator layers_handler::begin ()
{
  return layers_handler::layers_iterator (m_document->item_container (), m_layers_container.begin ());
}

layers_handler::layers_iterator layers_handler::end ()
{
  return layers_handler::layers_iterator (m_document->item_container (), m_layers_container.end ());
}

int layers_handler::layers_count ()
{
  return (int) m_layers_container.size ();
}

void layers_handler::toggle_layer_visibility (int layer_index)
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

void layers_handler::set_active_layer (int new_index)
{
  auto item_container = m_document->item_container ();
  m_active_layer_index = new_index;
  item_container->get_root ()->get_attribute_for_change<svg_attribute_active_layer> ()->set_value ((*(begin () + new_index))->get_computed_attribute<svg_attribute_layer_name> ()->value ());
  m_document->apply_changes ("Change Active Layer");
  emit active_layer_changed ();
}

int layers_handler::active_layer_index ()
{
  return m_active_layer_index;
}

void layers_handler::update_active_layer_index_by_attribute ()
{
  auto item_container = m_document->item_container ();
  auto active_layer_attr = item_container->get_root ()->get_computed_attribute<svg_attribute_active_layer> ();
  QString active_layer_name = active_layer_attr ? active_layer_attr->value () : QString ();
  auto active_layer = !active_layer_name.isNull () ? find_if (begin (), end (), [&](abstract_svg_item *l)
  {
    return active_layer_name == l->get_computed_attribute<svg_attribute_layer_name> ()->value ();
  })
    : end ();
  m_active_layer_index = active_layer - begin ();
}

void layers_handler::update_attribute_by_active_layer_index ()
{
  auto item_container = m_document->item_container ();
  auto attr = item_container->get_root ()->get_attribute_for_change<svg_attribute_active_layer> ();
  attr->set_value (get_active_layer_item ()->get_computed_attribute<svg_attribute_layer_name> ()->value ());
}

bool layers_handler::is_layer_visible (int index)
{
  return get_layer_item (index)->get_computed_attribute<svg_attribute_display> ()->value () != display::NONE;
  return true;
}

void layers_handler::rename_layer (int index, QString new_name)
{
  auto item = get_layer_item (index);
  if (item != m_document->root ())
    {
      item->get_attribute_for_change<svg_attribute_layer_name> ()->set_value (new_name);
      m_document->apply_changes ("Rename Layer");
    }
}

void layers_handler::move_layer (int from, int to)
{
  auto root = m_document->root ();
  auto item_from = get_layer_item (from);
  auto item_to = get_layer_item (to);
  if (item_from == root || to > m_layers_container.size ())
    return;

  root->move_child (item_to->child_index () + 1, item_from);
  slide (m_layers_container.begin () + from, m_layers_container.begin () + from + 1, m_layers_container.begin () + to);
  m_active_layer_index = from < to ? to - 1 : to;
  m_document->apply_changes ("Layer Moved");

  update_attribute_by_active_layer_index ();
  emit layers_changed ();
}

int layers_handler::get_layer_opacity (int index)
{
  auto item = get_layer_item (index);
  return (int) (item->get_computed_attribute<svg_attribute_opacity> ()->value () * 100.0);
}

int layers_handler::get_active_layer_opacity ()
{
  return get_layer_opacity (m_active_layer_index);
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
