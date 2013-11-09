#include "editor/handles_editor.h"

#include "common/memory_deallocation.h"

#include "editor/handles_renderer.h"
#include "editor/items_selection.h"

#include "svg/items/abstract_svg_item.h"

#include "renderer/svg_painter.h"
#include "gui/mouse_shortcuts_handler.h"
#include "abstract_handle.h"
#include "element_handles.h"


handles_editor::handles_editor (overlay_renderer *overlay, svg_painter *painter, mouse_shortcuts_handler *mouse_handler)
{
  m_overlay = overlay;
  m_painter = painter;
  m_mouse_handler = mouse_handler;
  m_cur_handle = nullptr;

  m_renderer = new handles_renderer (this, m_overlay, m_painter->item_container ());

  ADD_SHORTCUT_DRAG (m_mouse_handler, DRAG_HANDLE,
                     return start_drag (m_event.pos ()),
                     return drag_handle (m_event.pos ()),
                     return end_drag (m_event.pos ()));
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
  m_renderer->add_svg_item (item->name ());
}

void handles_editor::remove_item (const std::string &item)
{
  m_handles.erase (item);
  m_renderer->remove_svg_item (item);
}

void handles_editor::clear_items ()
{
  m_handles.clear ();
  m_renderer->clear_items ();
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
  for (const auto & handle_pair : m_handles)
    {
      abstract_handle *handle = get_handle_by_element (screen_pos.toPoint (), handle_pair.second.get ());
      if (handle)
        return handle;
    }

  return nullptr;
}

QPointF handles_editor::get_local_pos (QPointF screen_pos) const
{
  return m_painter->get_local_pos (screen_pos);
}

abstract_handle *handles_editor::get_handle_by_element (QPoint screen_pos, element_handles *element) const
{
  QTransform trans = m_painter->cur_transform ();
  for (const auto &handle : element->handles ())
    {
      if (handle->is_mouse_inside (screen_pos, trans))
        return handle;
    }

  return nullptr;
}