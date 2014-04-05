#include "gui/actions_applier.h"

#include "common/debug_utils.h"

#include "gui/gui_action_id.h"
#include "gui/shortcuts_config.h"
#include "gui/mouse_shortcuts_handler.h"
#include "gui/mouse_filter.h"


actions_applier::actions_applier ()
{
  m_actions.resize ((int)gui_action_id::COUNT);
  m_shortcuts.resize ((int) mouse_shortcut_t::COUNT);
  m_drag_shortcuts.resize ((int) mouse_drag_shortcut_t::COUNT);
}

actions_applier::~actions_applier ()
{

}

void actions_applier::register_action (gui_action_id id, action_type func)
{
  m_actions[(int)id].push_back (func);
}

bool actions_applier::apply_action (gui_action_id id)
{
  for (auto && action : m_actions[(int)id])
    if (action ())
      return true;

  return false;
}

void actions_applier::add_shortcut_func (mouse_shortcut_t shortcut, const mouse_shortcut_func_t &func)
{
  m_shortcuts[(int)shortcut].push_back (func);
}

void actions_applier::add_drag_shortcut_func (mouse_drag_shortcut_t shortcut, const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end)
{
  m_drag_shortcuts[(int)shortcut].push_back (drag_shortcut_func_t (start, drag, end));
}

bool actions_applier::apply_action (const mouse_event_t &m_event, mouse_shortcut_enum_union action)
{
  if (action.m_is_mouse_shortcut)
    {
      int shortcut_num = (int)action.m_shortcut;
      for (auto &&shortcut : m_shortcuts[shortcut_num])
        if (shortcut (m_event))
           return true;
    }
  else
    {
      int shortcut_num = (int)action.m_drag_shortcut;
      switch (m_event.event_type ())
        {
          case mouse_event_type::DRAG_START:
            for (auto &&shortcut : m_drag_shortcuts[shortcut_num])
              if (shortcut.m_start && shortcut.m_start (m_event))
                return true;

            return false;
          case mouse_event_type::DRAG_END:
            for (auto &&shortcut : m_drag_shortcuts[shortcut_num])
              if (shortcut.m_end && shortcut.m_end (m_event))
                return true;

            return false;
          case mouse_event_type::DRAG:
            for (auto &&shortcut : m_drag_shortcuts[shortcut_num])
              if (shortcut.m_drag && shortcut.m_drag (m_event))
                return true;

            return false;
          default:
            DEBUG_PAUSE ("Wrong event type");
            return false;
        }
    }
  return false;
}