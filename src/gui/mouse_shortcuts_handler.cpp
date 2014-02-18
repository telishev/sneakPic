#include "gui/mouse_shortcuts_handler.h"

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"

#include "gui/mouse_filter.h"
#include "gui/shortcuts_config.h"


mouse_shortcuts_handler::mouse_shortcuts_handler (const shortcuts_config *cfg, const mouse_callback_func_t &callback)
{
  m_cfg = cfg;
  m_mouse_callback = callback;
}

mouse_shortcuts_handler::~mouse_shortcuts_handler ()
{
}

bool mouse_shortcuts_handler::process_mouse_event (const mouse_event_t &m_event)
{
  if (!is_mouse_event_type_drag (m_event.event_type ()))
    return process_mouse_shortcuts (m_event);
  else
    return process_mouse_drag_shortcuts (m_event);
}

bool mouse_shortcuts_handler::process_mouse_shortcuts (const mouse_event_t &m_event)
{
  for (int i = 0; i < (int)mouse_shortcut_t::COUNT; i++)
    {
      mouse_shortcut shortcut = m_cfg->shortcut_mouse ((mouse_shortcut_t)i);
      if (shortcut.is_applicable (m_event))
        {
          if (m_mouse_callback (m_event, mouse_shortcut_enum_union ((mouse_shortcut_t)i)))
            return true;
        }
    }

  return false;
}

bool mouse_shortcuts_handler::process_mouse_drag_shortcuts (const mouse_event_t &m_event)
{
  switch (m_event.event_type ())
    {
      case mouse_event_type::DRAG_START: return process_mouse_drag_start (m_event);
      case mouse_event_type::DRAG_END: return process_mouse_drag_end (m_event);
      case mouse_event_type::DRAG: return process_mouse_drag (m_event);
      default:
        DEBUG_PAUSE ("Wrong event type");
        return false;
    }
}

bool mouse_shortcuts_handler::process_mouse_drag_start (const mouse_event_t &m_event)
{
  for (mouse_drag_shortcut_t shortcut : get_applicable_shortcuts (m_event, false))
    {
      if (m_mouse_callback (m_event, mouse_shortcut_enum_union (shortcut)))
        {
          m_drag_stack.push_back (shortcut);
          return true;
        }
    }

  return false;
}

bool mouse_shortcuts_handler::process_mouse_drag (const mouse_event_t &m_event)
{
  if (m_drag_stack.empty ())
    return false;

  return m_mouse_callback (m_event, mouse_shortcut_enum_union (m_drag_stack.back ()));
}

bool mouse_shortcuts_handler::process_mouse_drag_end (const mouse_event_t &m_event)
{
  for (mouse_drag_shortcut_t shortcut : get_applicable_shortcuts (m_event, true))
    {
      auto it = std::find (m_drag_stack.begin (), m_drag_stack.end (), shortcut);
      if (it == m_drag_stack.end ())
        continue;

      m_mouse_callback (m_event, mouse_shortcut_enum_union (shortcut));
      m_drag_stack.erase (it);
    }

  return false;
}

vector<mouse_drag_shortcut_t>
mouse_shortcuts_handler::get_applicable_shortcuts (const mouse_event_t &m_event, bool ignore_modifiers) const
{
  vector<mouse_drag_shortcut_t> result;
  for (int i = 0; i < (int)mouse_drag_shortcut_t::COUNT; i++)
    {
      mouse_shortcut shortcut = m_cfg->drag_shortcut_mouse ((mouse_drag_shortcut_t)i);
      if (shortcut.is_applicable (m_event, ignore_modifiers))
        {
          result.push_back ((mouse_drag_shortcut_t)i);
        }
    }

  return result;
}



