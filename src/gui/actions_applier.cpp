#include "gui/actions_applier.h"

#include "common/debug_utils.h"

#include "gui/gui_action_id.h"
#include "gui/shortcuts_config.h"
#include "gui/mouse_shortcuts_handler.h"
#include "gui/mouse_filter.h"


actions_applier::actions_applier ()
{
  m_actions.resize ((int)gui_action_id::COUNT);
  m_shortcuts.resize ((int) mouse_shortcut_enum::COUNT);
  m_drag_shortcuts.resize ((int) mouse_drag_shortcut_enum::COUNT);
}

actions_applier::~actions_applier ()
{

}

void actions_applier::register_action (gui_action_id id, action_type func)
{
  DEBUG_ASSERT (!m_actions[(int)id]);
  m_actions[(int)id] = func;
}

bool actions_applier::apply_action (gui_action_id id)
{
  if (!m_actions[(int)id])
    return false;

  return m_actions[(int)id] ();
}

void actions_applier::add_shortcut (mouse_shortcut_enum shortcut, const mouse_shortcut_func_t &func)
{
  DEBUG_ASSERT (!m_shortcuts[(int)shortcut]);
  m_shortcuts[(int)shortcut] = func;
}

void actions_applier::add_drag_shortcut (mouse_drag_shortcut_enum shortcut, const mouse_shortcut_func_t &start, const mouse_shortcut_func_t &drag, const mouse_shortcut_func_t &end)
{
  m_drag_shortcuts[(int)shortcut] = drag_shortcut_func_t (start, drag, end);
}

bool actions_applier::apply_action (const mouse_event_t &m_event, mouse_shortcut_enum_union action)
{
  if (action.m_is_mouse_shortcut)
    {
      int shortcut_num = (int)action.m_shortcut;
      if (m_shortcuts[shortcut_num] && m_shortcuts[shortcut_num] (m_event))
        return true;
    }
  else
    {
      int shortcut_num = (int)action.m_drag_shortcut;
      switch (m_event.event_type ())
        {
          case mouse_event_type::DRAG_START:
            if (m_drag_shortcuts[shortcut_num].m_start)
              return m_drag_shortcuts[shortcut_num].m_start (m_event);

            return false;
          case mouse_event_type::DRAG_END:
            if (m_drag_shortcuts[shortcut_num].m_end)
              return m_drag_shortcuts[shortcut_num].m_end (m_event);

            return false;
          case mouse_event_type::DRAG:
            if (m_drag_shortcuts[shortcut_num].m_drag)
              return m_drag_shortcuts[shortcut_num].m_drag (m_event);

            return false;
          default:
            DEBUG_PAUSE ("Wrong event type");
            return false;
        }
    }
  return false;
}