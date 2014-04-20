#include "editor/handles_editor.h"

#include "editor/handles_renderer.h"
#include "editor/abstract_handle.h"

#include "svg/items/abstract_svg_item.h"

#include "renderer/svg_painter.h"
#include "renderer/overlay_renderer.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/shortcuts_config.h"
#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"



handles_editor::handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier)
{
  m_overlay = overlay;
  m_painter = painter;
  m_applier = applier;
  m_cur_handle = nullptr;
  m_highlighted_handle = nullptr;

  put_in (m_renderer, this);
  m_overlay->add_item (m_renderer.get (), overlay_layer_type::TEMP);

  m_applier->add_drag_shortcut (mouse_drag_shortcut_t::DRAG_HANDLE, this,
                                &handles_editor::start_drag, &handles_editor::drag_handle, &handles_editor::end_drag);

  m_applier->add_shortcut (mouse_shortcut_t::HIGHLIGHT_HANDLE, this, &handles_editor::highlight_handle);

  m_applier->register_action (gui_action_id::INTERRUPT, this, &handles_editor::interrupt_all);
}

handles_editor::~handles_editor ()
{
}

void handles_editor::update_handles ()
{
  update_handles_impl ();
  m_cur_handle = nullptr;
  m_highlighted_handle = nullptr;
  highlight_handle (QCursor::pos ());
}

bool handles_editor::start_drag (QPointF pos)
{
  m_cur_handle = get_handle_by_pos (pos);
  if (!m_cur_handle)
    return false;

  return m_cur_handle->start_drag (get_local_pos (pos), m_painter->cur_transform ());
}

bool handles_editor::drag_handle (QPointF pos)
{
  if (m_cur_handle == nullptr)
    return false;

  if (m_cur_handle->drag (get_local_pos (pos), m_painter->cur_transform ()))
    {
      m_painter->update ();
      return true;
    }

  return false;
}

bool handles_editor::end_drag (QPointF pos)
{
  if (m_cur_handle == nullptr)
    return false;

  if (m_cur_handle->end_drag (get_local_pos (pos), m_painter->cur_transform ()))
    {
      m_painter->update ();
      return true;
    }

  return false;
}

abstract_handle *handles_editor::get_handle_by_pos (QPointF screen_pos) const
{
  const double max_distance = 10;
  double distance = max_distance;
  abstract_handle *handle = nullptr;
  auto handles = get_handles ();
  QTransform trans = m_painter->cur_transform ();

  for (auto priority : {handle_priority::NORMAL, handle_priority::LOW})
    {
      if (handle)
        break;

      for (auto cur_handle : handles)
        {
          if (cur_handle->priority () != priority)
            continue;

          double cur_distance = cur_handle->distance_to_mouse (screen_pos.toPoint (), trans);
          if (cur_distance <= distance)
            {
              distance = cur_distance;
              handle = cur_handle;
            }
        }
    }

  return handle;
}

QPointF handles_editor::get_local_pos (QPointF screen_pos) const
{
  return m_painter->get_local_pos (screen_pos);
}

bool handles_editor::highlight_handle (QPointF pos)
{
  abstract_handle *cur_handle = get_handle_by_pos (pos);
  if (cur_handle != m_highlighted_handle && m_highlighted_handle)
    {
      m_highlighted_handle->set_mouse_hovered (false);
      m_painter->update ();
    }

  m_highlighted_handle = cur_handle;
  if (!m_highlighted_handle)
    return false;

  m_highlighted_handle->set_mouse_hovered (true);
  m_painter->update ();
  return true;
}

bool handles_editor::interrupt_all ()
{
  if (m_cur_handle)
     m_cur_handle->interrupt_drag ();

  m_cur_handle = nullptr;
  return false;
}

void handles_editor::interrupt ()
{
  m_applier->apply_action (gui_action_id::INTERRUPT);
}


