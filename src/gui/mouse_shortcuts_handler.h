#ifndef MOUSE_SHORTCUTS_HANDLER_H
#define MOUSE_SHORTCUTS_HANDLER_H

#include <vector>
#include <functional>

#include "common/common_utils.h"

#include "gui/mouse_shortcut.h"
#include "gui/shortcuts_config.h"

class shortcuts_config;

typedef std::function<bool (const mouse_event_t &)> mouse_shortcut_func_t;

#define MOUSE_FUNC(FUNC) [&] (const mouse_event_t &m_event) { FIX_UNUSED (m_event); FUNC; }

#define ADD_SHORTCUT(HANDLER,ITEM,FUNC)\
  HANDLER->add_shortcut (mouse_shortcut_enum::ITEM, MOUSE_FUNC (FUNC));

#define ADD_SHORTCUT_DRAG(HANDLER,ITEM,START,DRAG,STOP)\
  HANDLER->add_drag_shortcut (mouse_drag_shortcut_enum::ITEM, MOUSE_FUNC (START), MOUSE_FUNC (DRAG), MOUSE_FUNC (STOP));

class mouse_shortcuts_handler
{
  struct drag_shortcut_func_t
  {
    mouse_shortcut_func_t m_start, m_drag, m_end;
    drag_shortcut_func_t () {}
    drag_shortcut_func_t (const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end)
      : m_start (start), m_drag (drag), m_end (end) {}
  };

  const shortcuts_config *m_cfg;
  mouse_shortcut_func_t m_shortcuts[mouse_shortcut_enum::COUNT];
  drag_shortcut_func_t m_drag_shortcuts[mouse_drag_shortcut_enum::COUNT];

  std::vector<const drag_shortcut_func_t *> m_drag_stack;

public:
  mouse_shortcuts_handler (const shortcuts_config *cfg);
  ~mouse_shortcuts_handler ();

  void add_shortcut (mouse_shortcut_enum shortcut, const mouse_shortcut_func_t &func);
  void add_drag_shortcut (mouse_drag_shortcut_enum shortcut, const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end);
  bool process_mouse_event (const mouse_event_t &m_event);

private:
  bool process_mouse_shortcuts (const mouse_event_t &m_event);
  bool process_mouse_drag_shortcuts (const mouse_event_t &m_event);
  bool process_mouse_drag_start (const mouse_event_t &m_event);
  bool process_mouse_drag (const mouse_event_t &m_event);
  bool process_mouse_drag_end (const mouse_event_t &m_event);

  std::vector<const drag_shortcut_func_t *> get_applicable_shortcuts (const mouse_event_t &m_event, bool ignore_modifiers) const;
};

#endif // MOUSE_SHORTCUTS_HANDLER_H
