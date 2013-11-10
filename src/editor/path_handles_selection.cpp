#include "editor/path_handles_selection.h"

#include <vector>

#include "editor/items_selection.h"

#include "gui/connection.h"

#include "svg/svg_document.h"
#include "svg/changed_items_container.h"
#include "svg/attributes/svg_attribute_type.h"
#include "svg/attributes/abstract_attribute.h"


path_anchors_selection::path_anchors_selection (items_selection *selection, svg_document *document)
{
  m_selection = selection;
  m_document = document;
  m_changes_started = false;

  items_edit_handler_t *edit_handler = m_document->items_edit_handler ();
  CONNECT (edit_handler, &items_edit_handler_t::attribute_change_start_signal, this, &path_anchors_selection::attribute_changed);
}

path_anchors_selection::~path_anchors_selection ()
{

}

void path_anchors_selection::add_anchor (const std::string &item, int id)
{
  m_selected_anchors[item].insert (id);
}

void path_anchors_selection::remove_anchor (const std::string &item, int id)
{
  m_selected_anchors[item].erase (id);
}

void path_anchors_selection::clear ()
{
  m_selected_anchors.clear ();
}

void path_anchors_selection::select_by_rect (const QRectF &/*rect*/)
{

}

void path_anchors_selection::update ()
{
  std::vector<std::string> removed_items;
  for (const auto &anchors_pair : m_selected_anchors)
    if (!m_selection->contains (anchors_pair.first))
      removed_items.push_back (anchors_pair.first);

  for (const auto &item : removed_items)
    m_selected_anchors.erase (item);
}

void path_anchors_selection::attribute_changed (const std::string &sender, const abstract_attribute *computed_attribute)
{
  if (m_changes_started)
    return;

  if (computed_attribute->type () != svg_attribute_type::D)
    return;

  m_selected_anchors.erase (sender);
}

bool path_anchors_selection::is_selected (const std::string &item, int id) const
{
  auto it = m_selected_anchors.find (item);
  if (it == m_selected_anchors.end ())
    return false;

  return it->second.find (id) != it->second.end ();
}

