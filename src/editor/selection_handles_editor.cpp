#include "editor/selection_handles_editor.h"

#include "common/memory_deallocation.h"

#include "editor/handles_renderer.h"
#include "editor/items_selection.h"
#include "editor/abstract_handle.h"
#include "editor/element_handles.h"

#include "svg/items/abstract_svg_item.h"

#include "renderer/svg_painter.h"
#include "renderer/overlay_renderer.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/shortcuts_config.h"
#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"



selection_handles_editor::selection_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier)
  : handles_editor (overlay, painter, applier)
{
}

selection_handles_editor::~selection_handles_editor ()
{
}

element_handles *selection_handles_editor::handles_for_item (const string &item) const
{
  auto it = m_handles.find (item);
  if (it == m_handles.end ())
    return nullptr;

  return it->second.get ();
}

void selection_handles_editor::add_item (abstract_svg_item *item)
{
  if (!item)
    return;

  element_handles *handle = create_handles_for_item (item);
  if (!handle)
    return;

  m_handles[item->name ()].reset (handle);
}

void selection_handles_editor::remove_item (const string &item)
{
  m_handles.erase (item);
}

void selection_handles_editor::clear_items ()
{
  m_handles.clear ();
}

void selection_handles_editor::set_handle_for_item (const char *name, element_handles *handles)
{
  m_handles[name].reset (handles);
}

void selection_handles_editor::update_handles_impl ()
{
  clear_items ();
  for (auto item : *m_painter->selection ())
    add_item (item);
}

std::vector<abstract_handle *> selection_handles_editor::get_handles () const
{
  std::vector<abstract_handle *> result;
  for (auto &&handles_pair : m_handles)
    {
      for (auto &&handle : handles_pair.second->handles ())
        result.push_back (handle);
    }

  return result;
}
