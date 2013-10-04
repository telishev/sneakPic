#ifndef SHORTCUTS_CONFIG_H
#define SHORTCUTS_CONFIG_H

#include "gui/mouse_shortcut.h"

enum class mouse_shortcut_enum
{
  SELECT_ITEM,
  ADD_ITEM_TO_SELECTION,
  FIND_CURRENT_OBJECT,

  COUNT,
};

enum class mouse_drag_shortcut_enum
{
  PAN,
  RUBBERBAND_SELECTION,

  COUNT,
};

class shortcuts_config
{
  mouse_shortcut m_mouse_shortcuts[(int)mouse_shortcut_enum::COUNT];
  mouse_shortcut m_drag_shortcuts[(int)mouse_drag_shortcut_enum::COUNT];

public:
  shortcuts_config ();
  ~shortcuts_config ();

  mouse_shortcut shortcut_mouse (mouse_shortcut_enum index) const { return m_mouse_shortcuts[(int)index]; }
  mouse_shortcut drag_shortcut_mouse (mouse_drag_shortcut_enum index) const { return m_drag_shortcuts[(int)index]; }

  void fill_by_default ();
};

#endif // SHORTCUTS_CONFIG_H
