#ifndef ACTIONS_APPLIER_H
#define ACTIONS_APPLIER_H

#include <vector>
#include <functional>

#include "common/common_utils.h"
#include "gui/shortcuts_config.h"

class mouse_shortcut_enum_union;
class mouse_event_t;

enum class gui_action_id;
enum class mouse_shortcut_enum;
enum class mouse_drag_shortcut_enum;

typedef std::function<bool (const mouse_event_t &)> mouse_shortcut_func_t;

class actions_applier
{
  struct drag_shortcut_func_t
  {
    mouse_shortcut_func_t m_start, m_drag, m_end;
    drag_shortcut_func_t () {}
    drag_shortcut_func_t (const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end)
      : m_start (start), m_drag (drag), m_end (end) {}
  };

  typedef std::function<bool ()> action_type;
  vector<action_type> m_actions;

  vector<mouse_shortcut_func_t> m_shortcuts;
  vector<drag_shortcut_func_t> m_drag_shortcuts;

public:
  actions_applier ();
  ~actions_applier ();

  void register_action (gui_action_id id, action_type func);

  template<typename T>
  void register_action (gui_action_id id, T *caller, bool (T::*func) ())
  {
    register_action (id, std::bind (func, caller));
  }

  bool apply_action (gui_action_id id);

  template<typename T, typename FUNC_PARAM>
  void add_shortcut (mouse_shortcut_enum shortcut, T *caller, bool (T::*func) (FUNC_PARAM))
  {
    add_shortcut_func (shortcut, [=] (const mouse_event_t &m_event) { return (caller->*func) ((FUNC_PARAM)m_event); });
  }

  template<typename T, typename FUNC_PARAM1, typename FUNC_PARAM2, typename FUNC_PARAM3>
  void add_drag_shortcut (mouse_drag_shortcut_enum shortcut, T *caller, bool (T::*start) (FUNC_PARAM1),
    bool (T::*drag) (FUNC_PARAM2), bool (T::*end) (FUNC_PARAM3))
  {
    add_drag_shortcut_func (shortcut, [=] (const mouse_event_t &m_event) { return (caller->*start) ((FUNC_PARAM1)m_event); },
                                      [=] (const mouse_event_t &m_event) { return (caller->*drag) ((FUNC_PARAM2)m_event); },
                                      [=] (const mouse_event_t &m_event) { return (caller->*end) ((FUNC_PARAM3)m_event); });
  }

  void add_shortcut_func (mouse_shortcut_enum shortcut, const mouse_shortcut_func_t &func);
  void add_drag_shortcut_func (mouse_drag_shortcut_enum shortcut, const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end);

  bool apply_action (const mouse_event_t &m_event, mouse_shortcut_enum_union action);
};

#endif // ACTIONS_APPLIER_H
