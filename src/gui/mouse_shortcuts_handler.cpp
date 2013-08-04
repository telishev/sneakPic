#include "gui/mouse_shortcuts_handler.h"




mouse_shortcuts_handler::mouse_shortcuts_handler ()
{

}

mouse_shortcuts_handler::~mouse_shortcuts_handler ()
{

}

void mouse_shortcuts_handler::add_shortcut (const mouse_shortcut &shortcut, const mouse_shortcut_func_t &func)
{
  m_shortcuts.push_back (shortcut_with_func (shortcut, func));
}

bool mouse_shortcuts_handler::process_mouse_event (const mouse_event_t &m_event) const
{
  for (const shortcut_with_func &shortcut : m_shortcuts)
    {
      if (shortcut.is_applicable (m_event))
        {
          shortcut.call (m_event);
          return true;
        }
    }

  return false;
}
