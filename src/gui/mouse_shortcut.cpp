#include "gui/mouse_shortcut.h"

#include "gui/mouse_filter.h"

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

bool mouse_shortcut::is_applicable (const mouse_event_t &m_event) const
{
  if (m_event.event_type () != m_mouse_type)
    return false;

  if (m_event.button () != m_button && m_button != mouse_button::ANY_BUTTON)
    return false;

  return m_modifiers.testFlag (m_event.modifier ());
}
