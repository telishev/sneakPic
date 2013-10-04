#include "gui/mouse_shortcuts_handler.h"

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"

#include "gui/mouse_filter.h"


mouse_shortcuts_handler::mouse_shortcuts_handler (const shortcuts_config *cfg)
{
  m_cfg = cfg;
}

mouse_shortcuts_handler::~mouse_shortcuts_handler ()
{
}

void mouse_shortcuts_handler::add_shortcut (mouse_shortcut_enum shortcut, const mouse_shortcut_func_t &func)
{
  DEBUG_ASSERT (!m_shortcuts[(int)shortcut]);
  m_shortcuts[(int)shortcut] = func;
}

void mouse_shortcuts_handler::add_drag_shortcut (mouse_drag_shortcut_enum shortcut, const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end)
{
  m_drag_shortcuts[(int)shortcut] = drag_shortcut_func_t (start, drag, end);
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
  for (int i = 0; i < (int)mouse_shortcut_enum::COUNT; i++)
    {
      mouse_shortcut shortcut = m_cfg->shortcut_mouse ((mouse_shortcut_enum)i);
      if (shortcut.is_applicable (m_event))
        {
          if (m_shortcuts[i] && m_shortcuts[i] (m_event))
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
  for (const drag_shortcut_func_t *shortcut : get_applicable_shortcuts (m_event, false))
    {
      if (shortcut->m_start (m_event))
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

  return m_drag_stack.back ()->m_drag (m_event);
}

bool mouse_shortcuts_handler::process_mouse_drag_end (const mouse_event_t &m_event)
{
  for (const drag_shortcut_func_t *shortcut : get_applicable_shortcuts (m_event, true))
    {
      auto it = std::find (m_drag_stack.begin (), m_drag_stack.end (), shortcut);
      if (it == m_drag_stack.end ())
        continue;

      shortcut->m_end (m_event);
      m_drag_stack.erase (it);
    }

  return false;
}

std::vector<const mouse_shortcuts_handler::drag_shortcut_func_t *>
mouse_shortcuts_handler::get_applicable_shortcuts (const mouse_event_t &m_event, bool ignore_modifiers) const
{
  std::vector<const drag_shortcut_func_t *> result;
  for (int i = 0; i < (int)mouse_drag_shortcut_enum::COUNT; i++)
    {
      mouse_shortcut shortcut = m_cfg->drag_shortcut_mouse ((mouse_drag_shortcut_enum)i);
      if (shortcut.is_applicable (m_event, ignore_modifiers))
        {
          if (m_drag_shortcuts[i].m_drag)
            result.push_back (&m_drag_shortcuts[i]);
        }
    }

  return result;
}


