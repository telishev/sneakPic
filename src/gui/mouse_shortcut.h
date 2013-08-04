#ifndef MOUSE_SHORTCUT_H
#define MOUSE_SHORTCUT_H

#include <QtCore>

class mouse_event_t;

enum class mouse_event_type
{
  INVALID,
  MOVE,
  CLICK,
  DOUBLECLICK,
  DRAG_START,
  DRAG,
  DRAG_END,

};

enum keyboard_modifier
{
  NO_MODIFIERS = 0x1,
  CTRL = 0x2,
  ALT = 0x4,
  SHIFT = 0x8,
  CTRL_SHIFT = 0x10,
  CTRL_ALT = 0x20,
  SHIFT_ALT = 0x40,
  CTRL_ALT_SHIFT = 0x80,

  ALL = 0xff,
};
Q_DECLARE_FLAGS (keyboard_modifiers, keyboard_modifier);

enum class mouse_button
{
  ANY_BUTTON = -2,
  NO_BUTTON = -1,

  BUTTON_LEFT = 0,
  BUTTON_RIGHT,
  BUTTON_MIDDLE,

  BUTTONS_COUNT,
};

class mouse_shortcut
{
  mouse_button m_button;
  keyboard_modifiers m_modifiers;
  mouse_event_type m_mouse_type;
public:
  mouse_shortcut ();
  mouse_shortcut (mouse_event_type mouse_type, mouse_button button, keyboard_modifiers modifiers);
  ~mouse_shortcut () {}

  bool is_applicable (const mouse_event_t &m_event) const;
};

#endif // MOUSE_SHORTCUT_H
