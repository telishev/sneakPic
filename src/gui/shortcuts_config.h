#ifndef SHORTCUTS_CONFIG_H
#define SHORTCUTS_CONFIG_H

#include "gui/mouse_shortcut.h"

enum class mouse_shortcut_enum
{
  SELECT_ITEM,
  START_PAN,
  PAN_PICTURE,
  END_PAN,
  FIND_CURRENT_OBJECT,

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
