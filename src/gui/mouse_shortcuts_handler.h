#ifndef MOUSE_SHORTCUTS_HANDLER_H
#define MOUSE_SHORTCUTS_HANDLER_H

#include <vector>
#include <functional>

class shortcuts_config;
class mouse_event_t;

enum class mouse_shortcut_t;
enum class mouse_drag_shortcut_t;

class mouse_shortcut_enum_union
{
public:
  mouse_shortcut_t m_shortcut;
  mouse_drag_shortcut_t m_drag_shortcut;
  bool m_is_mouse_shortcut;

  mouse_shortcut_enum_union (mouse_shortcut_t shortcut) : m_shortcut (shortcut), m_is_mouse_shortcut (true) {}
  mouse_shortcut_enum_union (mouse_drag_shortcut_t shortcut) : m_drag_shortcut (shortcut), m_is_mouse_shortcut (false) {}
};

typedef std::function<bool (const mouse_event_t &, mouse_shortcut_enum_union action)> mouse_callback_func_t;

class mouse_shortcuts_handler
{
  const shortcuts_config *m_cfg;
  vector<mouse_drag_shortcut_t> m_drag_stack;
  mouse_callback_func_t m_mouse_callback;

public:
  mouse_shortcuts_handler (const shortcuts_config *cfg, const mouse_callback_func_t &callback);
  ~mouse_shortcuts_handler ();

  bool process_mouse_event (const mouse_event_t &m_event);

private:
  bool process_mouse_shortcuts (const mouse_event_t &m_event);
  bool process_mouse_drag_shortcuts (const mouse_event_t &m_event);
  bool process_mouse_drag_start (const mouse_event_t &m_event);
  bool process_mouse_drag (const mouse_event_t &m_event);
  bool process_mouse_drag_end (const mouse_event_t &m_event);

  vector<mouse_drag_shortcut_t> get_applicable_shortcuts (const mouse_event_t &m_event, bool ignore_modifiers) const;
};

#endif // MOUSE_SHORTCUTS_HANDLER_H

