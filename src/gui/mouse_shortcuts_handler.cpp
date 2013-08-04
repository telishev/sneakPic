#include "gui/mouse_shortcuts_handler.h"

#include "common/debug_utils.h"
#include "common/memory_deallocation.h"


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

bool mouse_shortcuts_handler::process_mouse_event (const mouse_event_t &m_event) const
{
  for (int i = 0; i < (int)mouse_shortcut_enum::COUNT; i++)
    {
      mouse_shortcut shortcut = m_cfg->shortcut_mouse ((mouse_shortcut_enum)i);
      if (shortcut.is_applicable (m_event))
        {
          m_shortcuts[i] (m_event);
          return true;
        }
    }

  return false;
}
