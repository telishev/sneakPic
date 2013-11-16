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

#define MOUSE_FUNC(FUNC) [=] (const mouse_event_t &m_event) { FIX_UNUSED (m_event); FUNC; }

#define ADD_SHORTCUT(HANDLER,ITEM,FUNC)\
  HANDLER->add_shortcut (mouse_shortcut_enum::ITEM, MOUSE_FUNC (FUNC));

#define ADD_SHORTCUT_DRAG(HANDLER,ITEM,START,DRAG,STOP)\
  HANDLER->add_drag_shortcut (mouse_drag_shortcut_enum::ITEM, MOUSE_FUNC (START), MOUSE_FUNC (DRAG), MOUSE_FUNC (STOP));

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
  std::vector<action_type> m_actions;

  std::vector<mouse_shortcut_func_t> m_shortcuts;
  std::vector<drag_shortcut_func_t> m_drag_shortcuts;

public:
  actions_applier ();
  ~actions_applier ();

  void register_action (gui_action_id id, action_type func);

  template<typename T>
  void register_action (gui_action_id id, T *caller, bool (T::*func) ())
  {
    register_action (id, [caller, func] () { return (caller->*func) (); });
  }

  bool apply_action (gui_action_id id);

  void add_shortcut (mouse_shortcut_enum shortcut, const mouse_shortcut_func_t &func);
  void add_drag_shortcut (mouse_drag_shortcut_enum shortcut, const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end);

  bool apply_action (const mouse_event_t &m_event, mouse_shortcut_enum_union action);
};

#endif // ACTIONS_APPLIER_H
