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

inline bool is_mouse_event_type_drag (mouse_event_type type)
{
  switch (type)
    {
    case mouse_event_type::DRAG_START:
    case mouse_event_type::DRAG:
    case mouse_event_type::DRAG_END:
      return true;
    default:
      return false;
    }
}

enum class keyboard_modifier
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
  NONE = 0x0,
};
Q_DECLARE_FLAGS (keyboard_modifiers, keyboard_modifier);
Q_DECLARE_OPERATORS_FOR_FLAGS (keyboard_modifiers)

enum class mouse_button
{
  ANY_BUTTON = -2,
  NO_BUTTON = -1,

  BUTTON_LEFT = 0,
  BUTTON_RIGHT,
  BUTTON_MIDDLE,

  BUTTONS_COUNT,
};

bool contains_modifier (keyboard_modifier pressed, keyboard_modifier buttons);

class mouse_shortcut
{
  mouse_button m_button;
  keyboard_modifiers m_modifiers;
  mouse_event_type m_mouse_type;
public:
  mouse_shortcut ();
  mouse_shortcut (mouse_event_type mouse_type, mouse_button button, keyboard_modifiers modifiers);
  ~mouse_shortcut () {}

  bool is_applicable (const mouse_event_t &m_event, bool ignore_modifiers = false) const;
};

#endif // MOUSE_SHORTCUT_H
