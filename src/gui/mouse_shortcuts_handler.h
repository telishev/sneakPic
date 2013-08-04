#ifndef MOUSE_SHORTCUTS_HANDLER_H
#define MOUSE_SHORTCUTS_HANDLER_H

#include <vector>
#include <functional>

#include "gui/mouse_shortcut.h"

typedef std::function<void (const mouse_event_t &)> mouse_shortcut_func_t;

#define MOUSE_FUNC(FUNC) [&] (const mouse_event_t &m_event) { FUNC; }
#define MOUSE_SHORTCUT(MOUSE_EVENT_TYPE,MOUSE_BUTTON,KEYBOARD_MODIFIER) \
  mouse_shortcut (mouse_event_type::MOUSE_EVENT_TYPE, mouse_button::MOUSE_BUTTON, keyboard_modifier::KEYBOARD_MODIFIER)

class shortcut_with_func
{
  mouse_shortcut m_shortcut;
  mouse_shortcut_func_t m_function;
public:
  shortcut_with_func (const mouse_shortcut &shortcut, const mouse_shortcut_func_t &func)
    : m_shortcut (shortcut), m_function (func) {}

  bool is_applicable (const mouse_event_t &m_event) const { return m_shortcut.is_applicable (m_event); }
  void call (const mouse_event_t &m_event) const { m_function (m_event); }
};


class mouse_shortcuts_handler
{
  std::vector<shortcut_with_func> m_shortcuts;
public:
  mouse_shortcuts_handler ();
  ~mouse_shortcuts_handler ();

  void add_shortcut (const mouse_shortcut &shortcut, const mouse_shortcut_func_t &func);

  bool process_mouse_event (const mouse_event_t &m_event) const;
};

#endif // MOUSE_SHORTCUTS_HANDLER_H
