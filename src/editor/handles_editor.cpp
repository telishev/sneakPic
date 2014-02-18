#include "editor/handles_editor.h"

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



handles_editor::handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier)
{
  m_overlay = overlay;
  m_painter = painter;
  m_applier = applier;
  m_cur_handle = nullptr;
  m_highlighted_handle = nullptr;

  m_renderer = new handles_renderer (this);
  m_overlay->add_item (m_renderer, overlay_layer_type::TEMP);

  m_applier->add_drag_shortcut (mouse_drag_shortcut_t::DRAG_HANDLE, this,
                                &handles_editor::start_drag, &handles_editor::drag_handle, &handles_editor::end_drag);

  m_applier->add_shortcut (mouse_shortcut_t::HIGHLIGHT_HANDLE, this, &handles_editor::highlight_handle);
}

handles_editor::~handles_editor ()
{
  FREE (m_renderer);
}

void handles_editor::update_handles ()
{
  clear_items ();
  for (auto item : *m_painter->selection ())
    add_item (item);

  m_cur_handle = nullptr;
  m_highlighted_handle = nullptr;
  update_handles_impl ();
  highlight_handle (QCursor::pos ());
}

element_handles *handles_editor::handles_for_item (const std::string &item) const
{
  auto it = m_handles.find (item);
  if (it == m_handles.end ())
    return nullptr;

  return it->second.get ();
}

void handles_editor::add_item (abstract_svg_item *item)
{
  element_handles *handle = create_handles_for_item (item);
  if (!handle)
    return;

  m_handles[item->name ()].reset (handle);
}

void handles_editor::remove_item (const std::string &item)
{
  m_handles.erase (item);
}

void handles_editor::clear_items ()
{
  m_handles.clear ();
}

bool handles_editor::start_drag (QPointF pos)
{
  m_cur_handle = get_handle_by_pos (pos);
  if (!m_cur_handle)
    return false;

  return m_cur_handle->start_drag (get_local_pos (pos));
}

bool handles_editor::drag_handle (QPointF pos)
{
  if (!m_cur_handle)
    return false;

  return m_cur_handle->drag (get_local_pos (pos));
}

bool handles_editor::end_drag (QPointF pos)
{
  if (!m_cur_handle)
    return false;

  return m_cur_handle->end_drag (get_local_pos (pos));
}

abstract_handle *handles_editor::get_handle_by_pos (QPointF screen_pos) const
{
  const double max_distance = 10;
  double distance = max_distance;
  abstract_handle *handle = nullptr, *cur_handle = nullptr;
  for (const auto & handle_pair : m_handles)
    {
      cur_handle = get_handle_by_element (screen_pos.toPoint (), handle_pair.second.get (), distance);
      if (cur_handle)
        handle = cur_handle;
    }

  return handle;
}

QPointF handles_editor::get_local_pos (QPointF screen_pos) const
{
  return m_painter->get_local_pos (screen_pos);
}

abstract_handle *handles_editor::get_handle_by_element (QPoint screen_pos, element_handles *element, double &distance) const
{
  QTransform trans = m_painter->cur_transform ();
  abstract_handle *cur_handle = nullptr;
  for (const auto &handle : element->handles ())
    {
      double cur_distance = handle->distance_to_mouse (screen_pos, trans);
      if (cur_distance <= distance)
        {
          distance = cur_distance;
          cur_handle = handle;
        }
    }

  return cur_handle;
}

bool handles_editor::highlight_handle (QPointF pos)
{
  abstract_handle *cur_handle = get_handle_by_pos (pos);
  if (cur_handle != m_highlighted_handle && m_highlighted_handle)
    m_highlighted_handle->set_mouse_hovered (false);

  m_highlighted_handle = cur_handle;
  if (!m_highlighted_handle)
    return false;

  m_highlighted_handle->set_mouse_hovered (true);
  m_painter->update ();
  return true;
}

void handles_editor::set_handle_for_item (const char *name, element_handles *handles)
{
  m_handles[name].reset (handles);
}
