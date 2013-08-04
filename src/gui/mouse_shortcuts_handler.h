#ifndef MOUSE_SHORTCUTS_HANDLER_H
#define MOUSE_SHORTCUTS_HANDLER_H

#include <vector>
#include <functional>

#include "gui/mouse_shortcut.h"
#include "gui/shortcuts_config.h"

class shortcuts_config;

typedef std::function<void (const mouse_event_t &)> mouse_shortcut_func_t;

#define MOUSE_FUNC(FUNC) [&] (const mouse_event_t &m_event) { FUNC; }

class mouse_shortcuts_handler
{
  const shortcuts_config *m_cfg;
  mouse_shortcut_func_t m_shortcuts[mouse_shortcut_enum::COUNT];
public:
  mouse_shortcuts_handler (const shortcuts_config *cfg);
  ~mouse_shortcuts_handler ();

  void add_shortcut (mouse_shortcut_enum shortcut, const mouse_shortcut_func_t &func);
  bool process_mouse_event (const mouse_event_t &m_event) const;
};

#endif // MOUSE_SHORTCUTS_HANDLER_H
