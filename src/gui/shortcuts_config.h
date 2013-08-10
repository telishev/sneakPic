#ifndef SHORTCUTS_CONFIG_H
#define SHORTCUTS_CONFIG_H

#include "gui/mouse_shortcut.h"

enum class mouse_shortcut_enum
{
  SELECT_ITEM,
  ADD_ITEM_TO_SELECTION,
  START_PAN,
  PAN_PICTURE,
  END_PAN,
  FIND_CURRENT_OBJECT,
  START_RUBBERBAND_SELECTION,
  MOVE_RUBBERBAND_SELECTION,
  END_RUBBERBAND_SELECTION,

  COUNT,
};

class shortcuts_config
{
  mouse_shortcut m_mouse_shortcuts[(int)mouse_shortcut_enum::COUNT];

public:
  shortcuts_config ();
  ~shortcuts_config ();

  mouse_shortcut shortcut_mouse (mouse_shortcut_enum index) const { return m_mouse_shortcuts[(int)index]; }

  void fill_by_default ();
};

#endif // SHORTCUTS_CONFIG_H
