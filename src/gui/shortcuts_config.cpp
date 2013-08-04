#include "gui/shortcuts_config.h"

#include "gui/mouse_shortcuts_handler.h"

#define MOUSE_SHORTCUT(MOUSE_EVENT_TYPE,MOUSE_BUTTON,KEYBOARD_MODIFIER) \
  mouse_shortcut (mouse_event_type::MOUSE_EVENT_TYPE, mouse_button::MOUSE_BUTTON, keyboard_modifier::KEYBOARD_MODIFIER)

#define SET_MOUSE_SHORTCUT(INDEX,MOUSE_EVENT_TYPE,MOUSE_BUTTON,KEYBOARD_MODIFIER) \
  m_mouse_shortcuts[(int)mouse_shortcut_enum::INDEX] = MOUSE_SHORTCUT (MOUSE_EVENT_TYPE, MOUSE_BUTTON, KEYBOARD_MODIFIER);

shortcuts_config::shortcuts_config ()
{

}

shortcuts_config::~shortcuts_config ()
{

}

void shortcuts_config::fill_by_default ()
{
  SET_MOUSE_SHORTCUT (SELECT_ITEM        , CLICK     , BUTTON_LEFT  , NO_MODIFIERS);
  SET_MOUSE_SHORTCUT (START_PAN          , DRAG_START, BUTTON_MIDDLE, ALL         );
  SET_MOUSE_SHORTCUT (PAN_PICTURE        , DRAG      , BUTTON_MIDDLE, ALL         );
  SET_MOUSE_SHORTCUT (FIND_CURRENT_OBJECT, MOVE      , ANY_BUTTON   , ALL         );
}


