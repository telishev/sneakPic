#include "gui/shortcuts_config.h"

#include "common/common_utils.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/actions_info.h"
#include "gui/gui_action_id.h"



#define ADD_MODIFIER(MODIFIER) keyboard_modifier::MODIFIER |

#define MOUSE_SHORTCUT(MOUSE_EVENT_TYPE,MOUSE_BUTTON,...) \
  EXPAND (mouse_shortcut (mouse_event_type::MOUSE_EVENT_TYPE, mouse_button::MOUSE_BUTTON, FOR_EACH (ADD_MODIFIER, __VA_ARGS__) keyboard_modifier::NONE))

#define SET_MOUSE_SHORTCUT(INDEX,MOUSE_EVENT_TYPE,MOUSE_BUTTON,...) \
  EXPAND (m_mouse_shortcuts[(int)mouse_shortcut_enum::INDEX] = MOUSE_SHORTCUT (MOUSE_EVENT_TYPE, MOUSE_BUTTON, __VA_ARGS__);)

#define SET_MOUSE_DRAG_SHORTCUT(INDEX,MOUSE_EVENT_TYPE,MOUSE_BUTTON,...) \
  EXPAND (m_drag_shortcuts[(int)mouse_drag_shortcut_enum::INDEX] = MOUSE_SHORTCUT (MOUSE_EVENT_TYPE, MOUSE_BUTTON, __VA_ARGS__);)

shortcuts_config::shortcuts_config ()
{
  m_mouse_shortcuts.resize ((int)mouse_shortcut_enum::COUNT);
  m_drag_shortcuts.resize ((int)mouse_drag_shortcut_enum::COUNT);
  m_action_shortcuts.resize ((int)gui_action_id::COUNT);
}

shortcuts_config::~shortcuts_config ()
{

}

void shortcuts_config::fill_by_default ()
{
  fill_mouse_default ();
  fill_drag_default ();
  fill_action_default ();
}

void shortcuts_config::fill_mouse_default ()
{
///------------------------------------------------------------------------------------
///                  |    SHORTCUT_NAME         | EVENT_TYPE|    BUTTON    | MODIFIERS
  SET_MOUSE_SHORTCUT (SELECT_ITEM               , CLICK     , BUTTON_LEFT  , NO_MODIFIERS);
  SET_MOUSE_SHORTCUT (ADD_ITEM_TO_SELECTION     , CLICK     , BUTTON_LEFT  , SHIFT       );
  SET_MOUSE_SHORTCUT (FIND_CURRENT_OBJECT       , MOVE      , ANY_BUTTON   , ALL         );
}

void shortcuts_config::fill_drag_default ()
{

  ///-----------------------------------------------------------------------------------------
  ///                     |    SHORTCUT_NAME    | EVENT_TYPE|    BUTTON    | MODIFIERS
  SET_MOUSE_DRAG_SHORTCUT (PAN                  , DRAG_START, BUTTON_MIDDLE, ALL                  );
  SET_MOUSE_DRAG_SHORTCUT (RUBBERBAND_SELECTION , DRAG_START, BUTTON_LEFT  , NO_MODIFIERS, SHIFT  );
  SET_MOUSE_DRAG_SHORTCUT (DRAG_OBJECTS         , DRAG_START, BUTTON_LEFT  , NO_MODIFIERS         );
}

void shortcuts_config::fill_action_default ()
{
  actions_info info;
  for (int i = 0; i < (int)gui_action_id::COUNT; i++)
    m_action_shortcuts[i] = info.default_shortcut ((gui_action_id)i);
}



