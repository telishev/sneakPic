#include "gui/mouse_shortcut.h"

#include "gui/mouse_filter.h"
#include "common/debug_utils.h"

mouse_shortcut::mouse_shortcut (mouse_event_type mouse_type, mouse_button button, keyboard_modifiers modifiers)
{
  m_button = button;
  m_mouse_type = mouse_type;
  m_modifiers = modifiers;
}

mouse_shortcut::mouse_shortcut ()
{
  m_button = mouse_button::ANY_BUTTON;
  m_mouse_type = mouse_event_type::INVALID;
  m_modifiers = keyboard_modifier::ALL;
}

bool mouse_shortcut::is_applicable (const mouse_event_t &m_event, bool ignore_modifiers) const
{
  if (!ignore_modifiers)
    {
      if (m_event.event_type () != m_mouse_type)
        return false;
    }

  if (m_event.button () != m_button && m_button != mouse_button::ANY_BUTTON)
    return false;

  if (ignore_modifiers)
    return true;
  else
    return m_modifiers.testFlag (m_event.modifier ());
}

bool contains_modifier (keyboard_modifier pressed, keyboard_modifier buttons)
{
  if (buttons == NO_MODIFIERS)
    return true;

  switch (pressed)
    {
    case NO_MODIFIERS: return false;
    case CTRL: return buttons == CTRL;
    case ALT: return buttons == ALT;
    case SHIFT: return buttons == SHIFT;
    case CTRL_SHIFT: return buttons == SHIFT ||  buttons == CTRL ||  buttons == CTRL_SHIFT;
    case CTRL_ALT: return buttons == ALT ||  buttons == CTRL ||  buttons == CTRL_ALT;
    case SHIFT_ALT: return buttons == SHIFT ||  buttons == ALT ||  buttons == SHIFT_ALT;
    case CTRL_ALT_SHIFT: return true;
    default: break;
    }

  DEBUG_PAUSE ("wrong button");
  return false;
}
