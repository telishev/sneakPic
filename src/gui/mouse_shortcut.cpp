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
  if (buttons == keyboard_modifier::NO_MODIFIERS)
    return true;

  switch (pressed)
    {
    case keyboard_modifier::NO_MODIFIERS: return false;
    case keyboard_modifier::CTRL: return buttons == keyboard_modifier::CTRL;
    case keyboard_modifier::ALT: return buttons == keyboard_modifier::ALT;
    case keyboard_modifier::SHIFT: return buttons == keyboard_modifier::SHIFT;
    case keyboard_modifier::CTRL_SHIFT: return buttons == keyboard_modifier::SHIFT ||  buttons == keyboard_modifier::CTRL ||  buttons == keyboard_modifier::CTRL_SHIFT;
    case keyboard_modifier::CTRL_ALT: return buttons == keyboard_modifier::ALT ||  buttons == keyboard_modifier::CTRL ||  buttons == keyboard_modifier::CTRL_ALT;
    case keyboard_modifier::SHIFT_ALT: return buttons == keyboard_modifier::SHIFT ||  buttons == keyboard_modifier::ALT ||  buttons == keyboard_modifier::SHIFT_ALT;
    case keyboard_modifier::CTRL_ALT_SHIFT: return true;
    default: break;
    }

  DEBUG_PAUSE ("wrong button");
  return false;
}
