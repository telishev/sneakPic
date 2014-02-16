#include "gui/shortcuts_config.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/actions_info.h"
#include "gui/gui_action_id.h"


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
  typedef mouse_shortcut_enum m;
  typedef mouse_event_type e;
  typedef mouse_button b;
  typedef keyboard_modifier k;
  set_mouse_shortcut (m::SELECT_ITEM                 , e::CLICK  , b::BUTTON_LEFT , k::NO_MODIFIERS);
  set_mouse_shortcut (m::ADD_ITEM_TO_SELECTION       , e::CLICK  , b::BUTTON_LEFT , k::SHIFT       );
  set_mouse_shortcut (m::FIND_CURRENT_OBJECT         , e::MOVE   , b::ANY_BUTTON  , k::ALL         );
  set_mouse_shortcut (m::HIGHLIGHT_HANDLE            , e::MOVE   , b::ANY_BUTTON  , k::ALL         );
  set_mouse_shortcut (m::SELECT_HANDLE               , e::CLICK  , b::BUTTON_LEFT , k::ALL         );
  set_mouse_shortcut (m::PEN_ADD_SEGMENT_SIMPLE      , e::CLICK  , b::BUTTON_LEFT , k::ALL         );
  set_mouse_shortcut (m::CHANGE_HANDLE_TYPE          , e::CLICK  , b::BUTTON_LEFT , k::CTRL        );
  set_mouse_shortcut (m::UPDATE_AUXILIARY_PEN_PREVIEW, e::MOVE   , b::ANY_BUTTON  , k::ALL         );
}

void shortcuts_config::fill_drag_default ()
{
  typedef mouse_drag_shortcut_enum m;
  typedef mouse_button b;
  typedef keyboard_modifier k;

  set_mouse_drag_shortcut (m::PAN                  , b::BUTTON_MIDDLE, k::ALL                      );
  set_mouse_drag_shortcut (m::RUBBERBAND_SELECTION , b::BUTTON_LEFT  , k::NO_MODIFIERS | k::SHIFT  );
  set_mouse_drag_shortcut (m::HANDLES_SELECTION    , b::BUTTON_LEFT  , k::NO_MODIFIERS | k::SHIFT  );
  set_mouse_drag_shortcut (m::DRAG_OBJECTS         , b::BUTTON_LEFT  , k::NO_MODIFIERS             );
  set_mouse_drag_shortcut (m::CREATE_RECTANGLE     , b::BUTTON_LEFT  , k::ALL                      );
  set_mouse_drag_shortcut (m::DRAG_HANDLE          , b::BUTTON_LEFT  , k::ALL                      );
  set_mouse_drag_shortcut (m::PEN_ADD_SEGMENT_DRAG , b::BUTTON_LEFT  , k::ALL                      );
  set_mouse_drag_shortcut (m::COLOR_PICKER         , b::BUTTON_RIGHT , k::ALL                      );
}

void shortcuts_config::fill_action_default ()
{
  actions_info info;
  for (int i = 0; i < (int)gui_action_id::COUNT; i++)
    m_action_shortcuts[i] = info.default_shortcut ((gui_action_id)i);
}

void shortcuts_config::set_mouse_shortcut (mouse_shortcut_enum index, mouse_event_type type, mouse_button button, keyboard_modifiers modifiers)
{
  m_mouse_shortcuts[(int)index] = mouse_shortcut (type, button, modifiers);
}

void shortcuts_config::set_mouse_drag_shortcut (mouse_drag_shortcut_enum index,  mouse_button button, keyboard_modifiers modifiers)
{
  m_drag_shortcuts[(int)index] = mouse_shortcut (mouse_event_type::DRAG_START, button, modifiers);
}



